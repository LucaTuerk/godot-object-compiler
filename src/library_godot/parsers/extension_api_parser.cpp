
#include "extension_api_parser.h"

#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/identifier.h"

static const char* core_interface = R"(
    namespace godot
    {
        enum ModuleInitializationLevel {
            MODULE_INITIALIZATION_LEVEL_CORE,
            MODULE_INITIALIZATION_LEVEL_SERVERS,
            MODULE_INITIALIZATION_LEVEL_SCENE,
            MODULE_INITIALIZATION_LEVEL_EDITOR,
            MODULE_INITIALIZATION_LEVEL_MAX
        };

        template <typename T>
        class Ref {};

        template <typename T>
        class TypedArray {};

        template <typename T, typename A = DefaultAllocator>
        class List {};

        template <typename K, typename V>
        class TypedDictionary {};

        class Variant {};

        class PropertyInfo {};

    } // namespace godot

    #define ADD_PROPERTY
    #define ADD_GROUP
    #define ADD_SUBGROUP
    #define GDCLASS
    #define GDREGISTER_CLASS
    #define GDREGISTER_VIRTUAL_CLASS
    #define GDREGISTER_ABSTRACT_CLASS
    #define GDREGISTER_INTERNAL_CLASS
    #define GDREGISTER_RUNTIME_CLASS
    #define GDVIRTUAL_CALL
    #define GDVIRTUAL_BIND
    #define BIND_ENUM_CONSTANT
    #define BIND_BITFIELD_FLAG
    #define VARIANT_ENUM_CAST
    #define VARIANT_BITFIELD_CAST
)";

namespace GodotObjectCompiler
{

    String class_name_to_canonical_name(const String& p_class_name)
    {
        StreamWriter writer;
        for (char c : p_class_name) {
            if (std::isalpha(c) && std::isupper(c)) {
                writer.write_generic(static_cast<char>(std::tolower(c)));
            } else if (c != '_') {
                writer.write_generic(c);
            }
        }
        return writer.get_string();
    }

    String path_to_canonical_name(const String& p_path)
    {
        return class_name_to_canonical_name(path_stem(p_path));
    }

    bool validate_header_path(const String& p_header_path, const String& p_godot_cpp_path)
    {
        String include_path = path_concat(p_godot_cpp_path, "include");
        String gen_include_path = path_concat(path_concat(p_godot_cpp_path, "gen"), "include");

        return file_exists(path_concat(include_path, p_header_path)) ||
               file_exists(path_concat(gen_include_path, p_header_path));
    }

    JsonError::JsonError(ErrorLevel p_level, const Json& p_json, const String& p_message)
    {
        error_level = p_level;
        message = format("JsonError: %s\n%s", p_message.c_str());
    }

    Ref<ParserError> ExtensionAPIParser::parse(const String& p_input, Ref<Context> r_target)
    {
        PARSER_ERROR_COND(!r_target, "ExtensionAPIParser: Invalid null target context.");

        std::ifstream file(p_input);
        Json json;
        try {
            json = Json::parse(file);
        } catch (std::exception& e) {
            PARSER_ERROR(
                "ExtensionAPIParser: Exception occurred parsing file \"%s\": %s", p_input.c_str(),
                e.what());
        }

        Ref<Body> body;
        r_target->B<Body>()[B<Namespace>()[{B<Identifier>("godot"), R<Body>(&body)}]];

        if (json.contains("global_enums")) {
            for (auto& element : json["global_enums"]) {
                try {
                    auto result = parse_enum(element);
                    result.has_result();
                    RESULT_ERROR_PASS_ON(JsonError, result, global_enum);
                    body->add_child(global_enum);
                } catch (std::exception& e) {
                    JSON_PARSE_ERR(
                        element, "Exception occurred while processing json: %s", e.what());
                }
            }
        }

        if (json.contains("builtin_classes")) {
            for (auto& element : json["builtin_classes"]) {
                try {
                    if (auto result = parse_class(element); result.has_result()) {
                        body->add_child(result.get_result());
                    } else {
                        result.get_error()->set_handled();
                    }
                } catch (std::exception& e) {
                    JSON_PARSE_ERR(
                        element, "Exception occurred while processing json: %s", e.what());
                }
            }
        }

        if (json.contains("classes")) {
            for (auto& element : json["classes"]) {
                try {
                    auto result = parse_class(element);
                    RESULT_ERROR_PASS_ON(JsonError, result, godot_class);
                    body->add_child(godot_class);
                } catch (std::exception& e) {
                    JSON_PARSE_ERR(
                        element, "Exception occurred while processing json: %s", e.what());
                }
            }
        }

        TreeSitterParser tree_sitter_parser;
        Ref<Context> core_context = node_new<Namespace>();
        Ref<ParserError> error = tree_sitter_parser.parse(core_interface, core_context);
        if (error != ParserError::OK) {
            return error;
        }

        for (Ref<Class> core_class : core_context->find_children<Class>(true)) {
            auto name = core_class->name();
            auto include_itr = include_paths.find(class_name_to_canonical_name(name));
            PARSER_ERROR_COND(
                include_itr == include_paths.end(), "Could not find include path for class \"%s\"",
                name.c_str());
            core_class->header = include_itr->second;
        }
        r_target->add_child(core_context);

        return ParserError::OK;
    }

    bool ExtensionAPIParser::setup_include_paths(const String& p_godot_cpp_path)
    {
        include_paths.clear();

        const Vector<String> paths = {
            path_concat(p_godot_cpp_path, "include"),
            path_concat(path_concat(p_godot_cpp_path, "gen"), "include")};

        for (const String& include_path : paths) {
            if (!directory_exits(include_path)) {
                fmt_print_err("Include directory \"%s\" does not exist.", include_path.c_str());
                return false;
            }

            for (const String& file : directory_files_recursive(include_path)) {
                include_paths[path_to_canonical_name(file)] = header_path(include_path, file);
            }
        }
        return true;
    }

    Result<Class, JsonError> ExtensionAPIParser::parse_class(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension class is not named.");

        String name = p_input["name"];
        auto include_itr = include_paths.find(class_name_to_canonical_name(name));

        JSON_PARSE_ERR_COND(
            include_itr == include_paths.end(), p_input,
            "Could not find include path for class \"%s\"", name.c_str());

        String include_path = include_itr->second;

        Ref<Class> result = B<Class>()[B<Identifier>(name)];

        if (p_input.contains("inherits")) {
            String base_name = p_input["inherits"];
            result->B<BaseClasses>()[B<Type>()[B<Identifier>(base_name)]];
        }

        Ref<Body> body = result->B<Body>();
        if (p_input.contains("enums")) {
            for (auto& element : p_input["enums"]) {
                if (auto enum_result = parse_enum(element); enum_result.has_result()) {
                    body->add_child(enum_result.get_result());
                } else {
                    return enum_result.get_error();
                }
            }
        }

        result->header = include_path;
        for (const Ref<NamedContext>& descendant : result->find_children<NamedContext>(true)) {
            descendant->header = include_path;
        }

        return result;
    }

    Result<Enum, JsonError> ExtensionAPIParser::parse_enum(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension enum is not named.");

        String name = string_replace(p_input["name"], ".", "::");
        bool is_variant_type = name == "Variant::Type";

        Ref<EnumValues> enum_values;
        Ref<Enum> result = B<Enum>()[{B<Identifier>(name), R<EnumValues>(&enum_values)}];

        if (p_input.contains("values")) {
            for (auto& element : p_input["values"]) {
                auto value_result = parse_enum_value(element);
                RESULT_ERROR_PASS_ON(JsonError, value_result, value);

                enum_values->add_child(value);

                if (is_variant_type) {
                    Ref<Identifier> identifier = value->find_descendant<Identifier>();
                    if (identifier) {
                        identifier->name = string_replace(identifier->name, "TYPE_", "");
                    }
                }
            }
        }
        return result;
    }

    Result<EnumValue, JsonError> ExtensionAPIParser::parse_enum_value(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension enum value is not named.");
        JSON_PARSE_ERR_COND(
            !p_input.contains("value"), p_input,
            "Parsed extension enum value does not have a value.");

        String name = p_input["name"];
        int value = p_input["value"];
        Ref<EnumValue> result =
            B<EnumValue>()[{B<Identifier>(name), B<Literal>(format("%d", value))}];
        return result;
    }
} // namespace GodotObjectCompiler
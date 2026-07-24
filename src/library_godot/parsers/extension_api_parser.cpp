/**************************************************************************/
/* extension_api_parser.cpp                                               */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "extension_api_parser.h"

#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/core/temp_file.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/identifier.h"

static const auto core_interface = R"(
    using DefaultAllocator = int;

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

    String path_to_canonical_name(const Path& p_path)
    {
        return class_name_to_canonical_name(p_path.stem().generic_string());
    }

    JsonError::JsonError(const ErrorLevel p_level, const Json& p_json, const String& p_message)
    {
        UNUSED(p_json); // TODO: Should be included in error message in collapsed form
        error_level = p_level;
        message = format("JsonError: %s", p_message.c_str());
    }

    Ref<ParserError> ExtensionAPIParser::parse_file(const Path& p_path, Ref<Context> r_target)
    {
        PARS_ERR_COND(!r_target, "ExtensionAPIParser: Invalid null target context.");

        std::ifstream file(p_path);
        Json json;
        try {
            json = Json::parse(file);
        } catch (std::exception& e) {
            PARS_ERR(
                "ExtensionAPIParser: Exception occurred parsing file \"%s\": %s", p_path.c_str(),
                e.what());
        }

        Ref<Body> body;
        r_target->B<Body>()[B<Namespace>()[{B<Identifier>("godot"), R<Body>(&body)}]];

        if (json.contains("global_enums")) {
            for (auto& element : json["global_enums"]) {
                auto result = parse_enum(element);
                RESULT_ERR_PASS_ON(JsonError, result, global_enum);
                body->add_child(global_enum);
            }
        }

        if (json.contains("builtin_classes")) {
            for (auto& element : json["builtin_classes"]) {
                if (auto result = parse_class(element); result.has_result()) {
                    body->add_child(result.get_result());
                } else {
                    result.get_error()->set_handled();
                }
            }
        }

        if (json.contains("classes")) {
            for (auto& element : json["classes"]) {
                auto result = parse_class(element);
                RESULT_ERR_PASS_ON(JsonError, result, godot_class);
                body->add_child(godot_class);
            }
        }

        Ref<IParser> source_parser = LibraryContext::instance()->get_default_parser(SOURCE_PARSER);
        Ref<Context> core_context = node_new<Namespace>();
        if (Ref<ParserError> error = source_parser->parse(core_interface, core_context);
            error != ParserError::OK) {
            return error;
        }

        for (const Ref<Class>& core_class : core_context->find_children<Class>(true)) {
            auto name = core_class->name();
            auto include_itr = include_paths.find(class_name_to_canonical_name(name));
            PARS_ERR_COND(
                include_itr == include_paths.end(), "Could not find include path for class \"%s\"",
                name.c_str());
            core_class->header = include_itr->second;
        }
        r_target->add_child(core_context);

        return ParserError::OK;
    }

    Ref<ParserError> ExtensionAPIParser::parse(const String& p_input, Ref<Context> r_target)
    {
        const TempFile temp_file("json", p_input);
        return parse_file(temp_file.get_path(), r_target);
    }

    bool ExtensionAPIParser::setup_include_paths(const Vector<Path>& p_godot_cpp_include)
    {
        include_paths.clear();

        for (const Path& include_path : p_godot_cpp_include) {
            if (!directory_exits(include_path)) {
                fmt_print_err("Include directory \"%s\" does not exist.", include_path.c_str());
                return false;
            }

            for (const Path& file : directory_files_recursive(include_path)) {
                include_paths[path_to_canonical_name(file)] = header_path(include_path, file);
            }
        }
        return true;
    }

    Result<Class, JsonError> ExtensionAPIParser::parse_class(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension class is not named.");

        try {
            const String name = p_input["name"];
            const auto include_itr = include_paths.find(class_name_to_canonical_name(name));

            JSON_PARSE_ERR_COND(
                include_itr == include_paths.end(), p_input,
                "Could not find include path for class \"%s\"", name.c_str());

            const String include_path = include_itr->second;

            Ref<Class> result = B<Class>()[B<Identifier>(name)];

            if (p_input.contains("inherits")) {
                const String base_name = p_input["inherits"];
                result->B<BaseClasses>()[B<Type>()[B<Identifier>(base_name)]];
            }

            const Ref<Body> body = result->B<Body>();
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
        } catch (const std::exception& e) {
            JSON_PARSE_ERR(p_input, "Exception occurred parsing class: %s", e.what());
        }
    }

    Result<Enum, JsonError> ExtensionAPIParser::parse_enum(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension enum is not named.");

        try {
            const String name = string_replace(p_input["name"], ".", "::");
            const bool is_variant_type = name == "Variant::Type";

            Ref<EnumValues> enum_values;
            Ref<Enum> result = B<Enum>()[{B<Identifier>(name), R<EnumValues>(&enum_values)}];

            if (p_input.contains("values")) {
                for (auto& element : p_input["values"]) {
                    auto value_result = parse_enum_value(element);
                    RESULT_ERR_PASS_ON(JsonError, value_result, value);

                    enum_values->add_child(value);

                    if (is_variant_type) {
                        if (const Ref<Identifier> identifier =
                                value->find_descendant<Identifier>()) {
                            identifier->name = string_replace(identifier->name, "TYPE_", "");
                        }
                    }
                }
            }
            return result;
        } catch (const std::exception& e) {
            JSON_PARSE_ERR(p_input, "Exception occurred parsing enum: %s", e.what());
        }
    }

    Result<EnumValue, JsonError> ExtensionAPIParser::parse_enum_value(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension enum value is not named.");
        JSON_PARSE_ERR_COND(
            !p_input.contains("value"), p_input,
            "Parsed extension enum value does not have a value.");

        try {
            const String name = p_input["name"];
            int value = p_input["value"];
            Ref<EnumValue> result =
                B<EnumValue>()[{B<Identifier>(name), B<Literal>(format("%d", value))}];
            return result;
        } catch (const std::exception& e) {
            JSON_PARSE_ERR(p_input, "Exception occurred parsing enum value: %s", e.what());
        }
    }
} // namespace GodotObjectCompiler

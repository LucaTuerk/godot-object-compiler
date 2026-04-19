
#include "extension_api_parser.h"

#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler
{
    JsonError::JsonError(ErrorLevel p_level, const Json& p_json, const String& p_message)
    {
        error_level = p_level;
        message = format("JsonError: %s\n%s", p_message.c_str(), p_json.dump(1).c_str());
    }

    Ref<ParserError> ExtensionAPIParser::parse(const String& p_input, Ref<Context> r_target)
    {
        if (!r_target) {
            return node_new<ParserError>(ERROR, "ExtensionAPIParser: Invalid null target context.");
        }

        std::ifstream file(p_input);
        Json json;
        try {
            json = Json::parse(file);
        } catch (std::exception& e) {
            return node_new<ParserError>(
                ERROR,
                format(
                    "ExtensionAPIParser: Exception occurred while processing file \"%s\": %s",
                    p_input.c_str(), e.what()));
        }

        Ref<Body> body;
        r_target->B<Body>()[B<Namespace>()[{B<Identifier>("godot"), R<Body>(&body)}]];

        if (json.contains("global_enums")) {
            for (auto& element : json["global_enums"]) {
                try {
                    if (auto result = parse_enum(element); result.has_result()) {
                        body->add_child(result.get_result());
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
                    if (auto result = parse_class(element); result.has_result()) {
                        body->add_child(result.get_result());
                    }
                } catch (std::exception& e) {
                    JSON_PARSE_ERR(
                        element, "Exception occurred while processing json: %s", e.what());
                }
            }
        }

        return ParserError::OK;
    }

    Result<Class, JsonError> ExtensionAPIParser::parse_class(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension class is not named.");

        String name = p_input["name"];

        Ref<Class> result = B<Class>()[B<Identifier>(name)];

        if (p_input.contains("inherits")) {
            String base_name = p_input["inherits"];
            result->B<Identifier>(base_name);
        }

        Ref<Body> body = result->B<Body>();
        if (p_input.contains("enums")) {
            for (auto& element : p_input["enums"]) {
                if (auto enum_result = parse_enum(element); enum_result.has_result()) {
                    body->add_child(enum_result.get_result());
                }
            }
        }

        return result;
    }

    Result<Enum, JsonError> ExtensionAPIParser::parse_enum(const Json& p_input)
    {
        JSON_PARSE_ERR_COND(
            !p_input.contains("name"), p_input, "Parsed extension enum is not named.");

        String name = string_replace(p_input["name"], ".", "::");
        Ref<EnumValues> enum_values;
        Ref<Enum> result = B<Enum>()[{B<Identifier>(name), R<EnumValues>(&enum_values)}];

        if (p_input.contains("values")) {
            for (auto& element : p_input["values"]) {
                auto value_result = parse_enum_value(element);
                if (value_result.has_result()) {
                    enum_values->add_child(value_result.get_result());
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
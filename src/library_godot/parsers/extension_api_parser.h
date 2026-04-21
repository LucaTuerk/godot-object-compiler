
#pragma once
#include "library/parser/parser.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler
{
    class JsonError : public ParserError
    {
      public:
        JsonError(ErrorLevel p_level, const String& p_message) : ParserError(p_level, p_message) {};
        JsonError(ErrorLevel p_level, const Json& p_json, const String& p_message);
    };

    class ExtensionAPIParser : public IParser
    {
      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        bool setup_include_paths(const String& p_godot_cpp_path);

      private:
        Result<Class, JsonError> parse_class(const Json& p_input);

        static Result<Node, JsonError> parse_global_enum(const Json& p_input);

        static Result<Enum, JsonError> parse_enum(const Json& p_input);

        static Result<EnumValue, JsonError> parse_enum_value(const Json& p_input);

        Dictionary<String, String> include_paths;
    };
} // namespace GodotObjectCompiler

#define JSON_PARSE_ERR(json, ...) return node_new<JsonError>(ERROR, json, format(__VA_ARGS__))

#define JSON_PARSE_ERR_COND(condition, json, ...)                                                  \
    do {                                                                                           \
        if (condition) {                                                                           \
            JSON_PARSE_ERR(json, __VA_ARGS__);                                                     \
        }                                                                                          \
    } while (false)
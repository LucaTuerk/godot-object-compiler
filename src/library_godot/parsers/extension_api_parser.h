/**************************************************************************/
/* extension_api_parser.h                                                 */
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

#pragma once
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"
#include "library/tree_sitter_parser/parser.h"

namespace GodotObjectCompiler
{
    class JsonError : public ParserError
    {
      public:
        JsonError(ErrorLevel p_level, const String& p_message) : ParserError(p_level, p_message){};
        JsonError(ErrorLevel p_level, const Json& p_json, const String& p_message);
    };

    class ExtensionAPIParser : public IParser
    {
      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        bool setup_include_paths(const Vector<String>& p_godot_cpp_include);

      private:
        Result<Class, JsonError> parse_class(const Json& p_input);

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

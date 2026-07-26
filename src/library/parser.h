/**************************************************************************/
/* parser.h                                                               */
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
#include "tree/syntax/parser_error.h"

namespace GodotObjectCompiler
{
    class IParser
    {
      public:
        enum Capabilities {
            CAPABILITIES_NONE = 0,
            SOURCE_PARSER = 1,
            SUPPORT_MACRO_EXPANSION = 1 << 1,
            JSON_CONFIG_PARSER = 1 << 15,
        };

        enum Config { CONFIG_PARSE_ATTRIBUTES, CONFIG_SKIP_ATTRIBUTES };

        virtual ~IParser() = default;

        virtual String get_type() = 0;

        virtual Ref<ParserError> parse_file(const Path& p_path, Ref<Context> r_target) = 0;

        virtual Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) = 0;

        virtual void config(Config p_config);

        virtual int get_capabilities();
    };
} // namespace GodotObjectCompiler

#define PARSER(type)                                                                               \
    static inline bool __source_parser_registered = LibraryContext::add_register_callback(         \
        [](LibraryContext* p_context) { p_context->register_source_parser<type>(); });             \
                                                                                                   \
  public:                                                                                          \
    String get_type() override                                                                     \
    {                                                                                              \
        return #type;                                                                              \
    };                                                                                             \
    static String get_type_static()                                                                \
    {                                                                                              \
        return #type;                                                                              \
    }                                                                                              \
                                                                                                   \
  private:

#define CAPABILITIES(capas)                                                                        \
  public:                                                                                          \
    int get_capabilities() override                                                                \
    {                                                                                              \
        return capas;                                                                              \
    }                                                                                              \
                                                                                                   \
  private:

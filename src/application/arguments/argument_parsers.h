/**************************************************************************/
/* argument_parsers.h                                                     */
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
#include "flag_argument.h"
#include "path_argument.h"
#include "string_argument.h"

#if GOC_LIBCLANG_PARSER_ENABLED
#include "library/parsers/libclang/parser.h"
#endif

#if GOC_TREE_SITTER_PARSER_ENABLED
#include "library/parsers/tree-sitter/parser.h"
#endif

namespace GodotObjectCompiler::CommandLineArgumentParsers
{
    static inline Ref<PathCommandLineArgumentParser> Path =
        make_ref<PathCommandLineArgumentParser>();

    static inline Ref<PathListCommandLineArgumentParser> PathList =
        make_ref<PathListCommandLineArgumentParser>();

    static inline Ref<StringCommandLineArgumentParser> String =
        make_ref<StringCommandLineArgumentParser>();

    static inline Ref<StringListCommandLineArgumentParser> StringList =
        make_ref<StringListCommandLineArgumentParser>();

    static inline Ref<FlagCommandLineArgumentParser<ErrorLevel>> LogLevel =
        make_ref<FlagCommandLineArgumentParser<ErrorLevel>>(
            FlagCommandLineArgumentParser<ErrorLevel>::InitList({
                {"Error", ERROR},
                {"Warning", WARNING},
                {"Info", INFO},
                {"Verbose", VERBOSE},
            }));

    static inline Ref<FlagCommandLineArgumentParser<ErrorDetail>> LogDetail =
        make_ref<FlagCommandLineArgumentParser<ErrorDetail>>(
            FlagCommandLineArgumentParser<ErrorDetail>::InitList(
                {{"Condensed", ErrorDetail::CONDENSED}, {"Full", ErrorDetail::FULL}}));

    static inline Ref<FlagCommandLineArgumentParser<GodotObjectCompiler::String>> SourceParser =
        make_ref<FlagCommandLineArgumentParser<GodotObjectCompiler::String>>(
            FlagCommandLineArgumentParser<GodotObjectCompiler::String>::InitList({
#if GOC_LIBCLANG_PARSER_ENABLED
                {ClangParser::get_type_static(), ClangParser::get_type_static()},
#endif
#if GOC_TREE_SITTER_PARSER_ENABLED
                {TreeSitterParser::get_type_static(), TreeSitterParser::get_type_static()},
#endif
            }));

} // namespace GodotObjectCompiler::CommandLineArgumentParsers

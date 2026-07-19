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

namespace GodotObjectCompiler::CLIArgs
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

    class ApplicationArgs : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> log_level = CommandLineArgument::defaulted(
            LogLevel, "log_level", "L", "The log level to use.", INFO);

        Ref<CommandLineArgument> log_detail = CommandLineArgument::defaulted(
            LogDetail, "log_detail", "D", "The log detail to use.", FULL);

        Ref<CommandLineArgument> source_parser = CommandLineArgument::optional(
            CLIArgs::SourceParser, "source_parser", "SP",
            "The name of the source parser to be used by godot-object-compiler.");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {log_level, log_detail, source_parser};
        }
    };

    class ApplicationArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> goc_path = CommandLineArgument::defaulted(
            CLIArgs::Path, "goc_path", "P",
            "The directory that will be used by godot-object-compiler for caching.", ".goc");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {goc_path};
        }
    };

    class GeneratorArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> type_db_path = CommandLineArgument::defaulted(
            CLIArgs::Path, "type_db_path", "T",
            "The directory that will be used by godot-object-compiler for TypeDB caching.",
            ".goc/cache");

        Ref<CommandLineArgument> generated_path = CommandLineArgument::defaulted(
            CLIArgs::Path, "generated_path", "G",
            "The directory that will be used by godot-object-compiler for generated files.",
            ".goc/generated");

        Ref<CommandLineArgument> include_paths = CommandLineArgument::defaulted(
            CLIArgs::PathList, "include_paths", "I",
            "The generators include paths, comma seperated. Must include the godot-cpp include and "
            "gen/include paths.",
            {});

        Ref<CommandLineArgument> root_path = CommandLineArgument::required(
            CLIArgs::Path, "root_path", "R", "The projects root path.");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {type_db_path, generated_path, include_paths, root_path};
        }
    };

    class GDExtensionProjectAruments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> sources = CommandLineArgument::defaulted(
            CLIArgs::PathList, "sources", "S",
            "The generators target source file paths, comma seperated.", {});

        Ref<CommandLineArgument> godot_cpp = CommandLineArgument::required(
            CLIArgs::PathList, "godot_cpp", "GPP",
            "The path to the godot-cpp repositories root used with your extension.");

        Ref<CommandLineArgument> extension_api = CommandLineArgument::required(
            CLIArgs::Path, "extension_api", "E",
            "The path to the extension api json file to be used with your extension.");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {sources, godot_cpp, extension_api};
        }
    };

} // namespace GodotObjectCompiler::CLIArgs

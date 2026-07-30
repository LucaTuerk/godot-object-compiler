/**************************************************************************/
/* argument_lists.h                                                       */
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
#include "argument.h"
#include "argument_parsers.h"

namespace GodotObjectCompiler
{

    class ApplicationArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> log_level = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::ErrorLevel, "log_level", "L", "The log level to use.",
            INFO);

        Ref<CommandLineArgument> log_detail = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::ErrorDetail, "log_detail", "D", "The log detail to use.",
            FULL);

        Ref<CommandLineArgument> source_parser = CommandLineArgument::optional(
            CommandLineArgumentParsers::SourceParser, "source_parser", "SP",
            "The name of the source parser to be used by godot-object-compiler.");

        Ref<CommandLineArgument> goc_path = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::Path, "goc_path", "P",
            "The directory that will be used by godot-object-compiler for caching.", ".goc");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {log_level, log_detail, source_parser, goc_path};
        }
    };

    class GeneratorArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> project_type = CommandLineArgument::required(
            CommandLineArgumentParsers::ProjectType, "project_type", "PT",
            "The type of project the generator targets.");

        Ref<CommandLineArgument> root_path = CommandLineArgument::required(
            CommandLineArgumentParsers::Path, "root_path", "R", "The projects root path.");

        Ref<CommandLineArgument> include_paths = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::PathList, "include_paths", "I",
            "The generators include paths.", {});

        Ref<CommandLineArgument> type_db_path = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::Path, "type_db_path", "T",
            "The directory that will be used by godot-object-compiler for TypeDB caching.",
            ".goc/type_db");

        Ref<CommandLineArgument> generated_path = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::Path, "generated_path", "G",
            "The directory that will be used by godot-object-compiler for generated files.",
            ".goc/generated");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {project_type, root_path, include_paths, type_db_path, generated_path};
        }
    };

    class GDExtensionProjectArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> extension_name = CommandLineArgument::required(
            CommandLineArgumentParsers::String, "extension_name", "N", "The GDExtensions name.");

        Ref<CommandLineArgument> godot_cpp = CommandLineArgument::required(
            CommandLineArgumentParsers::PathList, "godot_cpp", "GPP",
            "The used godot-cpp submodules include paths.");

        Ref<CommandLineArgument> extension_api = CommandLineArgument::required(
            CommandLineArgumentParsers::Path, "extension_api", "E",
            "The path to the extension api json file to be used with your extension.");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {godot_cpp, extension_api};
        }
    };

    class ModuleProjectArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> module_name = CommandLineArgument::required(
            CommandLineArgumentParsers::String, "module_name", "N", "The modules name.");

        Ref<CommandLineArgument> godot_root = CommandLineArgument::required(
            CommandLineArgumentParsers::Path, "godot_root", "GR",
            "The root path of the godot editor source.");

        Ref<CommandLineArgument> type_db_includes = CommandLineArgument::required(
            CommandLineArgumentParsers::PathList, "type_db_includes", "TI",
            "The directories to include for parsing when generating the TypeDB");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {module_name, godot_root, type_db_includes};
        }
    };
} // namespace GodotObjectCompiler

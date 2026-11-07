/**************************************************************************/
/* init_extension.h                                                       */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of godot-object-compiler                */
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
#include "application/arguments/argument.h"
#include "application/arguments/argument_parsers.h"
#include "program.h"

namespace GodotObjectCompiler
{
    class InitExtensionArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> path = CommandLineArgument::unnamed(
            CommandLineArgumentParsers::Path, "The directory path to init the new GDExtension in.");

        Ref<CommandLineArgument> extension_name = CommandLineArgument::optional(
            CommandLineArgumentParsers::String, "extension_name", "N",
            "The optional extension name. If unset the directory name will be used.");

        Ref<CommandLineArgument> godot_cpp_git = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::String, "godot_cpp_git", "GPP",
            "The godot-cpp git repository to use.", "https://github.com/godotengine/godot-cpp.git");

        Ref<CommandLineArgument> godot_api_version = CommandLineArgument::defaulted(
            CommandLineArgumentParsers::String, "godot_api_version", "GV",
            "The godot api version to use.", "4.7");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {path, extension_name, godot_cpp_git, godot_api_version};
        }
    };

    class InitExtension : public IProgram
    {
        PROGRAM(InitExtension, "init/extension")
        READONLY_PROGRAM

      public:
        Ref<ProgramError> execute(ApplicationContext& p_context) override;

        [[nodiscard]] CommandLineArgumentParseResult
        register_required_arguments(ApplicationContext& p_context) const override;
    };
} // namespace GodotObjectCompiler

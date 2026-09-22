/**************************************************************************/
/* main.h                                                                 */
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
#define GOC_TEST_CONTEXT
#include "application/arguments/argument.h"
#include "application/arguments/argument_parsers.h"
#include "library/library_context.h"

using namespace GodotObjectCompiler;

class IntegrationTestsArgumentList : public ICommandLineArgumentList
{

  public:
    Ref<CommandLineArgument> extension_api = CommandLineArgument::required(
        CommandLineArgumentParsers::Path, "extension_api", "E",
        "The path to the extension api json to run test against.");

    Ref<CommandLineArgument> godot_cpp_includes = CommandLineArgument::required(
        CommandLineArgumentParsers::PathList, "godot_cpp_includes", "GPP",
        "The godot_cpp include paths.");

    [[nodiscard]] Vector<std::shared_ptr<CommandLineArgument>> get_arguments() const override
    {
        return {extension_api, godot_cpp_includes};
    }
};

class ModuleTestsArguments : public ICommandLineArgumentList
{
  public:
    Ref<CommandLineArgument> godot_root = CommandLineArgument::required(
        CommandLineArgumentParsers::Path, "godot_root", "GR",
        "The path to the godot repositories root.");

    Ref<CommandLineArgument> type_db_includes = CommandLineArgument::required(
        CommandLineArgumentParsers::PathList, "type_db_includes", "TI",
        "The include paths to use for generating the TypeDB.");

    [[nodiscard]] Vector<std::shared_ptr<CommandLineArgument>> get_arguments() const override
    {
        return {godot_root, type_db_includes};
    }
};

int main(int argc, char* argv[]);

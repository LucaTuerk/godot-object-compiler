/**************************************************************************/
/* argument_tests.h                                                       */
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
#include "application/arguments/flag_argument.h"
#include "application/arguments/path_argument.h"
#include "tests/test_registry.h"

using namespace GodotObjectCompiler;

GOC_TEST(ArgumentAccessors)
{
    Ref<CommandLineArgument> argument =
        CommandLineArgument::required(CommandLineArgumentParsers::Path, "test1", "t1", "d1");
    GOC_TEST_ASSERT(argument->is_required(), "");
    GOC_TEST_ASSERT(argument->get_name() == "test1", "");
    GOC_TEST_ASSERT(argument->get_short_name() == "t1", "");
    GOC_TEST_ASSERT(argument->get_description() == "d1", "");

    Ref<CommandLineArgument> argument2 =
        CommandLineArgument::optional(CommandLineArgumentParsers::Path, "test2", "t2", "d2");
    GOC_TEST_ASSERT(!argument2->is_required(), "");
    GOC_TEST_ASSERT(argument2->get_name() == "test2", "");
    GOC_TEST_ASSERT(argument2->get_short_name() == "t2", "");
    GOC_TEST_ASSERT(argument2->get_description() == "d2", "");

    Ref<CommandLineArgument> argument3 =
        CommandLineArgument::unnamed(CommandLineArgumentParsers::Path, "d3");
    GOC_TEST_ASSERT(argument3->is_unnamed(), "");
    GOC_TEST_ASSERT(argument3->get_name() == "", "");
    GOC_TEST_ASSERT(argument3->get_short_name() == "", "");
    GOC_TEST_ASSERT(argument3->get_description() == "d3", "");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(PathArgument)
{
    Ref<PathCommandLineArgumentParser> parser = make_ref<PathCommandLineArgumentParser>();
    Ref<CommandLineArgument> argument = CommandLineArgument::required(parser, "path", "P", "");

    Vector<Path> args = {"--path=test_path"};
    argument->parse_arguments(args);

    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Path>() == path_absolute("test_path"),
        "Failed to get path argument");

    args = {"-P=test_path2"};
    argument->parse_arguments(args);

    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Path>() == path_absolute("test_path2"),
        "Failed to get path argument");

    Vector<String> unnamed_paths = {"--some_names=hello", "test_path1", "test_path2", "test_path3"};
    Ref<CommandLineArgument> unnamed_arg = CommandLineArgument::unnamed(parser, "");
    unnamed_arg->parse_arguments(unnamed_paths);

    GOC_TEST_ASSERT(unnamed_arg->size() == 3, "Invalid unnamed argument count.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(0) == path_absolute("test_path1"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(1) == path_absolute("test_path2"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(2) == path_absolute("test_path3"), "Invalid argument.");

    Vector<String> another = {
        "--some_names=hello", "another_test_path1", "another_test_path2", "another_test_path3"};
    unnamed_arg->parse_arguments(another);
    GOC_TEST_ASSERT(unnamed_arg->size() == 3, "Invalid unnamed argument count.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(0) == path_absolute("another_test_path1"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(1) == path_absolute("another_test_path2"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<String>(2) == path_absolute("another_test_path3"), "Invalid argument.");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(PathListArgument)
{
    Ref<PathListCommandLineArgumentParser> parser = make_ref<PathListCommandLineArgumentParser>();
    Ref<CommandLineArgument> argument = CommandLineArgument::required(parser, "paths", "P", "");

    Vector<Path> args = {"--paths=test_path,test_path2,test_path3"};
    argument->parse_arguments(args);

    GOC_TEST_ASSERT(argument->has_value(), "Failed to get path argument");
    auto paths = argument->get<Vector<Path>>();

    GOC_TEST_EQ(paths.size(), 3, "Invalid path count");
    GOC_TEST_EQ(paths[0], path_absolute("test_path"), "Invalid path 0");
    GOC_TEST_EQ(paths[1], path_absolute("test_path2"), "Invalid path 1");
    GOC_TEST_EQ(paths[2], path_absolute("test_path3"), "Invalid path 2");

    args = {"-P=test_path,test_path2,test_path3"};
    argument->parse_arguments(args);

    GOC_TEST_EQ(paths.size(), 3, "Invalid path count");
    GOC_TEST_EQ(paths[0], path_absolute("test_path"), "Invalid path 0");
    GOC_TEST_EQ(paths[1], path_absolute("test_path2"), "Invalid path 1");
    GOC_TEST_EQ(paths[2], path_absolute("test_path3"), "Invalid path 2");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(FlagArgument)
{
    enum Flags {
        FLAG_A,
        FLAG_B,
        FLAG_C,
    };
    const Ref<FlagCommandLineArgumentParser<Flags>> parser =
        make_ref<FlagCommandLineArgumentParser<Flags>>(std::initializer_list<Pair<String, Flags>>({
            {"FlagA", FLAG_A},
            {"FlagB", FLAG_B},
            {"FlagC", FLAG_C},
        }));
    Ref<CommandLineArgument> argument = CommandLineArgument::required(parser, "flag", "F", "");

    Vector<Path> args = {"--flag=FlagA"};
    argument->parse_arguments(args);
    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Flags>() == FLAG_A, "Failed to get flag argument");

    args = {"-F=FlagB"};
    argument->parse_arguments(args);
    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Flags>() == FLAG_B, "Failed to get flag argument");

    args = {"--flag=FlagC"};
    argument->parse_arguments(args);
    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Flags>() == FLAG_C, "Failed to get flag argument");

    args = {"--flag=FlagD"};
    argument->parse_arguments(args);
    GOC_TEST_ASSERT(!argument->has_value(), "Invalid flag, argument should not hold value.");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(MissingRequiredArguments)
{
    enum Flags {
        FLAG_A,
        FLAG_B,
        FLAG_C,
    };
    static const Ref<FlagCommandLineArgumentParser<Flags>> parser =
        make_ref<FlagCommandLineArgumentParser<Flags>>(std::initializer_list<Pair<String, Flags>>({
            {"FlagA", FLAG_A},
            {"FlagB", FLAG_B},
            {"FlagC", FLAG_C},
        }));

    class MissingRequiredArguments : public ICommandLineArgumentList
    {
      public:
        Ref<CommandLineArgument> required_path =
            CommandLineArgument::required(CommandLineArgumentParsers::Path, "path", "p", "");
        Ref<CommandLineArgument> required_string =
            CommandLineArgument::required(CommandLineArgumentParsers::String, "string", "s", "");
        Ref<CommandLineArgument> required_flag =
            CommandLineArgument::required(parser, "flag", "f", "");

        [[nodiscard]] Vector<Ref<CommandLineArgument>> get_arguments() const override
        {
            return {required_path, required_string, required_flag};
        }
    };

    ApplicationContext context;
    context.arguments = {};
    auto result = context.register_argument_lists<MissingRequiredArguments>();
    GOC_TEST_ASSERT(result.get_missing_arguments().size() == 3, "Invalid missing argument count.");
    GOC_TEST_ASSERT(result.get_error_message().size() > 0, "No error message supplied.");

    return TEST_RESULT_SUCCESS;
};

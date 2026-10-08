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

    Vector<String> args = {"--path=test_path"};
    argument->parse_arguments(args);

    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Path>() == path_absolute("test_path"),
        "Failed to get path argument");

    args = {"-P=test_path2"};
    argument->parse_arguments(args);

    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Path>() == path_absolute("test_path2"),
        "Failed to get path argument");

    const Vector<String> unnamed_paths = {
        "--some_names=hello", "test_path1", "test_path2", "test_path3"};
    Ref<CommandLineArgument> unnamed_arg = CommandLineArgument::unnamed(parser, "");
    unnamed_arg->parse_arguments(unnamed_paths);

    GOC_TEST_ASSERT(unnamed_arg->size() == 3, "Invalid unnamed argument count.");
    GOC_TEST_ASSERT(unnamed_arg->get<Path>(0) == path_absolute("test_path1"), "Invalid argument.");
    GOC_TEST_ASSERT(unnamed_arg->get<Path>(1) == path_absolute("test_path2"), "Invalid argument.");
    GOC_TEST_ASSERT(unnamed_arg->get<Path>(2) == path_absolute("test_path3"), "Invalid argument.");

    Vector<String> another = {
        "--some_names=hello", "another_test_path1", "another_test_path2", "another_test_path3"};
    unnamed_arg->parse_arguments(another);
    GOC_TEST_ASSERT(unnamed_arg->size() == 3, "Invalid unnamed argument count.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<Path>(0) == path_absolute("another_test_path1"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<Path>(1) == path_absolute("another_test_path2"), "Invalid argument.");
    GOC_TEST_ASSERT(
        unnamed_arg->get<Path>(2) == path_absolute("another_test_path3"), "Invalid argument.");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(PathArgumentAliased)
{
    const Ref<CommandLineArgument> argument =
        CommandLineArgument::required(CommandLineArgumentParsers::Path, "test1", "t1", "d1");
    const Ref<CommandLineArgument> argument2 =
        CommandLineArgument::optional(CommandLineArgumentParsers::Path, "test2", "t2", "d2");
    const Ref<CommandLineArgument> argument3 = CommandLineArgument::defaulted(
        CommandLineArgumentParsers::Path, "test3", "t3", "", "{alias:test1}/test3");
    const Ref<CommandLineArgument> argument4 = CommandLineArgument::defaulted(
        CommandLineArgumentParsers::Path, "test4", "t4", "", "{alias:test1/test4");

    const Vector<String> arguments = {"--test1=.", "--test2={alias:test1}/test2"};

    argument2->parse_arguments(arguments);
    GOC_TEST_ASSERT(
        !argument2->has_value(),
        "Test path 2 has value when alias Test path 1 in not yet available.");

    argument->parse_arguments(arguments);
    argument2->parse_arguments(arguments);
    argument3->parse_arguments(arguments);
    argument4->parse_arguments(arguments);

    GOC_TEST_ASSERT(
        argument->has_value() && argument->get<Path>() == path_cwd(),
        "Test path 1 was not parsed or is incorrect");
    GOC_TEST_ASSERT(
        argument2->has_value() && argument2->get<Path>() == (path_cwd() / "test2"),
        "Test path 2 was not parsed or is incorrect")
    GOC_TEST_ASSERT(
        argument3->has_value() && argument3->get<Path>() == (path_cwd() / "test3"),
        "Test path 3 was not parsed or is incorrect")
    GOC_TEST_ASSERT(
        argument4->has_value() && argument4->get<Path>() == (path_cwd() / "{alias:test1/test4"),
        "Invalid alias path has value.");

    const Ref<CommandLineArgument> list_base =
        CommandLineArgument::required(CommandLineArgumentParsers::Path, "list1", "l1", "d1");
    const Ref<CommandLineArgument> list =
        CommandLineArgument::required(CommandLineArgumentParsers::PathList, "list2", "l2", "d2");

    const Vector<String> list_arguments = {
        "--list1=.", "--list2={alias:list1}/test1,{alias:list1}/test2"};

    list->parse_arguments(list_arguments);
    GOC_TEST_ASSERT(
        !list->has_value(),
        "Path list argument has value when alias list base path is not yet available");

    list_base->parse_arguments(list_arguments);
    list->parse_arguments(list_arguments);

    GOC_TEST_ASSERT(
        list_base->has_value() && list_base->get<Path>() == path_cwd(),
        "Invalid list base path parsed.");
    GOC_TEST_ASSERT(
        list->has_value() && list->get<Vector<Path>>().size() == 2 &&
            list->get<Vector<Path>>()[0] == path_cwd() / "test1" &&
            list->get<Vector<Path>>()[1] == path_cwd() / "test2",
        "Invalid list paths parsed.");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(PathListArgument)
{
    Ref<PathListCommandLineArgumentParser> parser = make_ref<PathListCommandLineArgumentParser>();
    Ref<CommandLineArgument> argument = CommandLineArgument::required(parser, "paths", "P", "");

    Vector<String> args = {"--paths=test_path,test_path2,test_path3"};
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

    Vector<String> args = {"--flag=FlagA"};
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

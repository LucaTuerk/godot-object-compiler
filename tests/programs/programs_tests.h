/**************************************************************************/
/* programs_tests.h                                                       */
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
#include "application/application.h"
#include "library/core/file_system_utilities.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(Clear)
{
    Application application;
    const int result =
        application.run(TestRegistry::instance()->get_test_application_arguments({"clear"}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");

    const auto generated_files = directory_files_recursive(TestRegistry::get_generated_path());
    const auto cache_files = directory_files_recursive(TestRegistry::get_cache_path());

    for (const String& file : generated_files) {
        fmt_print_err("Generated File \"%s\" was not properly cleaned up.", file.c_str());
    }
    for (const String& file : cache_files) {
        fmt_print_err("TypeDB File \"%s\" was not properly cleaned up.", file.c_str());
    }

    if (!cache_files.empty() || !generated_files.empty()) {
        return TEST_RESULT_FAILURE;
    }

    return TEST_RESULT_SUCCESS;
};

GOC_INTEGRATION_TEST(ExportTypeDB)
{
    String exported_folder = path_concat(TestRegistry::get_test_root_dir(), "exported");

    Application application;
    const int result = application.run(TestRegistry::instance()->get_test_application_arguments(
        {"export", "type_db", exported_folder}));

    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    GOC_TEST_EQ(
        directory_files_recursive(TestRegistry::get_cache_path()).size(),
        directory_files_recursive(exported_folder).size(),
        "Invalid file count for exported TypeDB");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(Help)
{
    Application application;
    const int result =
        application.run(TestRegistry::instance()->get_test_application_arguments({"help"}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(InitLocalResources)
{
    Application application;
    const int result = application.run(
        TestRegistry::instance()->get_test_application_arguments({"init", "local_resources"}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");

    Vector<String> paths = {"variant_types", "macros"};

    for (const String& path : paths) {
        String abs_path = path_concat(TestRegistry::get_goc_path(), path);
        GOC_TEST_ASSERT(
            directory_exits(abs_path), "Expected directory %s does not exist.", abs_path.c_str());
        GOC_TEST_ASSERT(
            !directory_files(abs_path).empty(), "No resources generated in directory %s",
            abs_path.c_str());
    }
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(PrintParsed)
{
    Application application;
    String test_file = "tests/files/class_tests/simple_class.h";
    const int result = application.run(
        TestRegistry::instance()->get_test_application_arguments({"print", "parsed", test_file}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    return TEST_RESULT_SUCCESS;
};

GOC_INTEGRATION_TEST(PrintTransformed)
{
    Application application;
    String test_file = "tests/files/class_tests/simple_class.h";
    const int result = application.run(TestRegistry::instance()->get_test_application_arguments(
        {"print", "transformed", test_file}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    return TEST_RESULT_SUCCESS;
};

GOC_INTEGRATION_TEST(PrintType)
{
    Application application;
    const int result = application.run(
        TestRegistry::instance()->get_test_application_arguments({"print", "type", "Node3D"}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(Version)
{
    Application application;
    const int result =
        application.run(TestRegistry::instance()->get_test_application_arguments({"version"}));
    GOC_TEST_ASSERT(result == 0, "Failed to run program");
    return TEST_RESULT_SUCCESS;
};

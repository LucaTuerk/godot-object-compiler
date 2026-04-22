/**************************************************************************/
/* main.cpp                                                               */
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

#include "main.h"

#include "application/application.h"
#include "core/all.h"
#include "integration/all.h"
#include "library/all.h"
#include "library/core/permissions.h"
#include "parser/all.h"
#include "test_registry.h"

int main(int argc, char* argv[])
{
    bool run_integration_tests = false;
    for (int i = 1; i < argc; i++) {
        print_ln(argv[i]);
        if (String(argv[i]) == "run_integration_tests") {
            run_integration_tests = true;
        }
    }

    using namespace GodotObjectCompiler;
    LibraryContext::instance()->set_error_level(INFO, FULL);
    Permissions::instance()->add_write_path(".goc_tests");

    Size failed_count = 0;
    Size success_count = 0, ignore_count = 0, all_count = 0;
    for (const auto& [test_name, test_functor] : TestRegistry::instance()->get_tests()) {
        PRINT_INFO("Running test case \"%s\"", test_name.c_str());
        all_count++;

        const TestResult result = test_functor();
        switch (result) {
        case TEST_RESULT_SUCCESS:
            PRINT_INFO("%s\tSuccess!", test_name.c_str());
            print_ln(format("%s\tSuccess!", test_name.c_str()));
            success_count++;
            break;
        case TEST_RESULT_FAILURE:
            PRINT_INFO("%s\tFailed!", test_name.c_str());
            failed_count++;
            break;
        case TEST_RESULT_IGNORED:
            PRINT_INFO("%s\tIgnored!", test_name.c_str());
            ignore_count++;
            break;
        }
    }

    if (run_integration_tests) {
        Vector<String> include_paths;
        for (int i = 2; i < argc; i++) {
            if (i == 2) {
                TestRegistry::instance()->set_extension_api(argv[i]);
            } else {
                include_paths.emplace_back(argv[i]);
            }
        }
        TestRegistry::instance()->set_integration_tests_godot_cpp_include_paths(include_paths);

        for (const auto& [test_name, test_functor] :
             TestRegistry::instance()->get_integration_tests()) {
            PRINT_INFO("Running test case \"%s\"", test_name.c_str());
            {
                Application application;
                const Vector<String> args =
                    TestRegistry::instance()->get_test_application_arguments(
                        {"generate", "type_db"});
                PANIC_COND(application.run(args) != 0, "Failed to setup type db during test run.");
            }

            all_count++;
            const TestResult result = test_functor();
            switch (result) {
            case TEST_RESULT_SUCCESS:
                PRINT_INFO("%s\tSuccess!", test_name.c_str());
                print_ln(format("%s\tSuccess!", test_name.c_str()));
                success_count++;
                break;
            case TEST_RESULT_FAILURE:
                PRINT_INFO("%s\tFailed!", test_name.c_str());
                failed_count++;
                break;
            case TEST_RESULT_IGNORED:
                PRINT_INFO("%s\tIgnored!", test_name.c_str());
                ignore_count++;
                break;
            }
        }
    }

    PRINT_INFO(
        "Summary: %d failed, %d succeeded, %d ignored, %d tests run", failed_count, success_count,
        ignore_count, all_count);

    if (failed_count != 0) {
        return 1;
    }
    return 0;
}

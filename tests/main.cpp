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
#include "programs/all.h"
#include "test_registry.h"

constexpr Size NANO_TO_MILLIS = 1'000'000;

int main(int argc, char* argv[])
{

    Vector<String> failed_tests;

    bool run_integration_tests = false;
    for (int i = 1; i < argc; i++) {
        print_ln(argv[i]);
        if (String(argv[i]) == "run_integration_tests") {
            run_integration_tests = true;
        }
    }

    using namespace GodotObjectCompiler;
    LibraryContext::instance()->set_error_level(VERBOSE, FULL);

    Size failed_count = 0;
    Size success_count = 0, ignore_count = 0, all_count = 0;
    Dictionary<String, Size> parsers_total_times;

    for (const auto& parser : LibraryContext::instance()->get_parsers()) {
        if ((parser->get_capabilities() & IParser::SOURCE_PARSER) == 0) {
            continue;
        }

        {
            Application application;
            const Vector<String> args =
                TestRegistry::instance()->get_test_application_arguments({"clear"});
            PANIC_COND(
                application.run(args),
                "Failed to clear the cache before running tests with parser %s",
                parser->get_type().c_str());
        }
        Size timer_sum = 0;
        LibraryContext::instance()->set_default_parser(parser->get_type(), IParser::SOURCE_PARSER);

        PRINT_INFO("Running tests against parser: %s", parser->get_type().c_str());

        for (const auto& [test_name, test_functor] : TestRegistry::instance()->get_tests()) {
            PRINT_INFO("Running test case \"%s\"", test_name.c_str());
            all_count++;

            TestResult result = TEST_RESULT_FAILURE;
            Permissions::instance()->add_write_path(".goc_tests");
            TestTimer timer;
            try {
                result = test_functor();
            } catch (const std::exception& e) {
                print_err(e.what());
            }
            timer_sum += timer.elapsed_nanoseconds();

            switch (result) {
            case TEST_RESULT_SUCCESS:
                PRINT_INFO(
                    "%s (Parser: %s)\tSuccess! %dms", test_name.c_str(), parser->get_type().c_str(),
                    timer.elapsed_nanoseconds() / NANO_TO_MILLIS);
                success_count++;
                break;
            case TEST_RESULT_FAILURE:
                failed_tests.push_back(
                    format("%s (Parser: %s)", test_name.c_str(), parser->get_type().c_str()));
                PRINT_INFO(
                    "%s (Parser: %s)\tFailed!", test_name.c_str(), parser->get_type().c_str());
                failed_count++;
                break;
            case TEST_RESULT_IGNORED:
                PRINT_INFO(
                    "%s (Parser: %s)\tIgnored!", test_name.c_str(), parser->get_type().c_str());
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

                all_count++;
                PRINT_INFO("Running test case \"%s\"", test_name.c_str());
                {
                    Application application;
                    const Vector<String> args =
                        TestRegistry::instance()->get_test_application_arguments(
                            {"generate", "type_db"});
                    PANIC_COND(application.run(args), "Failed to setup type db during test run.");
                }

                Ref<IParser> source_parser =
                    LibraryContext::instance()->get_default_parser(IParser::SOURCE_PARSER);
                PANIC_COND(source_parser == nullptr, "Could not get source parser.");
                source_parser->config(IParser::CONFIG_PARSE_ATTRIBUTES);

                TestResult result = TEST_RESULT_FAILURE;
                Permissions::instance()->add_write_path(".goc_tests");
                TestTimer timer;
                try {
                    result = test_functor();
                } catch (const std::exception& e) {
                    print_err(e.what());
                }
                timer_sum += timer.elapsed_nanoseconds();

                switch (result) {
                case TEST_RESULT_SUCCESS:
                    PRINT_INFO(
                        "%s (Parser: %s)\tSuccess! %dms", test_name.c_str(),
                        parser->get_type().c_str(), timer.elapsed_nanoseconds() / NANO_TO_MILLIS);
                    success_count++;
                    break;
                case TEST_RESULT_FAILURE:
                    failed_tests.push_back(
                        format("%s (Parser: %s)", test_name.c_str(), parser->get_type().c_str()));
                    PRINT_INFO(
                        "%s (Parser: %s)\tFailed!", test_name.c_str(), parser->get_type().c_str());
                    failed_count++;
                    break;
                case TEST_RESULT_IGNORED:
                    PRINT_INFO(
                        "%s (Parser: %s)\tIgnored!", test_name.c_str(), parser->get_type().c_str());
                    ignore_count++;
                    break;
                }
            }
        }

        parsers_total_times[parser->get_type()] = timer_sum;
    }

    PRINT_INFO("\nTotal Execution Times:")
    PRINT_INFO(string_pad_right("", '-', 35).c_str());
    Size sum = 0;
    for (const auto& [parser_name, duration] : parsers_total_times) {
        sum += duration;
        PRINT_INFO(
            "%s%s", string_pad_right(parser_name, ' ', 20).c_str(),
            string_pad_left(format("%dms", duration / NANO_TO_MILLIS), ' ', 15).c_str());
    }
    PRINT_INFO(string_pad_right("", '-', 35).c_str());
    PRINT_INFO(
        "%s%s", string_pad_right("Sum", ' ', 20).c_str(),
        string_pad_left(format("%dms", sum / NANO_TO_MILLIS), ' ', 15).c_str());
    PRINT_INFO(
        "%s%s", string_pad_right("Average", ' ', 20).c_str(),
        string_pad_left(format("%dms", (sum / all_count) / NANO_TO_MILLIS), ' ', 15).c_str());

    PRINT_INFO(
        "\nSummary: %d failed, %d succeeded, %d ignored, %d tests run.", failed_count,
        success_count, ignore_count, all_count);

    for (const String& test_name : failed_tests) {
        PRINT_INFO("Failed %s", test_name.c_str());
    }

    if (failed_count != 0) {
        return 1;
    }
    return 0;
}

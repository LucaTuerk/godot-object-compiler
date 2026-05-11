/**************************************************************************/
/* application_tests.h                                                    */
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
#include "library/core/string_utilities.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

inline Vector<String> fuzz_arguments()
{
    Vector<String> args;
    for (int i = 0; i < 5; ++i) {
        args.push_back(generate_random_string(10));
    }
    return args;
}

GOC_TEST(ApplicationInvalidArgs)
{
    for (int i = 0; i < 100; i++) {
        Application application;
        auto args = fuzz_arguments();
        GOC_TEST_NEQ(
            application.run(fuzz_arguments()), 0, "Application run succeeded with fuzzing args %s",
            string_vector_combine(args, " ").c_str());
    }
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(EnsureSingleApplication)
{
    bool failed = false;

    Application application_a;

    try {
        Application application_b;
    } catch (std::exception& e) {
        failed = true;
    }

    GOC_TEST_ASSERT(failed, "Duplicate Application did not panic.");
    return TEST_RESULT_SUCCESS;
};
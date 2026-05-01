/**************************************************************************/
/* attribute_tests.h                                                      */
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
#include "library/tree/syntax/namespace.h"
#include "test_registry.h"

GOC_TEST(Parser_Attributes)
{
    GOC_TEST_PARSE_FILE("tests/files/attribute_tests/attributes.h");

    const Ref<Class> test_class = global_namespace->find_descendant<Class>();
    GOC_TEST_ASSERT(test_class != nullptr, "Test class not properly parsed.");
    GOC_TEST_EQ(test_class->name(), "Attributes", "Invalid name parsed for test class.");

    const Vector<Ref<Function>> functions = test_class->find_children<Function>(true);
    GOC_TEST_EQ(functions.size(), 3, "Invalid number of functions parsed.");
    GOC_TEST_EQ(functions[0]->name(), "a", "Invalid name for first function parsed.");
    GOC_TEST_EQ(functions[1]->name(), "b", "Invalid name for second function parsed.");
    GOC_TEST_EQ(functions[2]->name(), "c", "Invalid name for third function parsed.");

    const Vector<Ref<Parameter>> params = functions[1]->find_children<Parameter>(true);
    GOC_TEST_EQ(params.size(), 1, "Invalid number of parameters parsed.");
    GOC_TEST_EQ(params[0]->name(), "param", "Invalid name of parameter parsed.");

    return TEST_RESULT_SUCCESS;
};
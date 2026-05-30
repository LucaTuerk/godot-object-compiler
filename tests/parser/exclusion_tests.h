/**************************************************************************/
/* exclusion_tests.h                                                      */
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

#include "library/parsers/tree-sitter/parser.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/namespace.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_TEST(ParseExclusion_NoExclusion)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/no_exclusion.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class, found %d", classes.size());

    const Ref<Class>& test_class = classes[0];
    Vector<Ref<Field>> fields = test_class->find_children<Field>(true);
    GOC_TEST_ASSERT(fields.size() == 3, "Expected 3 fields (a, b, c), found %d", fields.size());

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseExclusion_Simple)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/simple_test.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class, found %d", classes.size());

    const Ref<Class>& test_class = classes[0];
    Vector<Ref<Field>> fields = test_class->find_children<Field>(true);
    GOC_TEST_ASSERT(fields.size() == 2, "Expected 2 fields (a, c), found %d", fields.size());

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseExclusion_BasicExclusion)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/basic_exclusion.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class, found %d", classes.size());

    const Ref<Class>& test_class = classes[0];
    Vector<Ref<Field>> fields = test_class->find_children<Field>(true);
    GOC_TEST_ASSERT(fields.size() == 2, "Expected 2 fields (a, c), found %d", fields.size());

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseExclusion_MultipleSections)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/multiple_sections.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class, found %d", classes.size());

    const Ref<Class>& test_class = classes[0];
    Vector<Ref<Field>> fields = test_class->find_children<Field>(true);
    GOC_TEST_ASSERT(fields.size() == 3, "Expected 3 fields (a, b, c), found %d", fields.size());

    Vector<String> field_names;
    for (const auto& field : fields) {
        if (!field->name().empty()) {
            field_names.push_back(field->name());
        }
    }

    bool has_a = std::find(field_names.begin(), field_names.end(), "a") != field_names.end();
    bool has_b = std::find(field_names.begin(), field_names.end(), "b") != field_names.end();
    bool has_c = std::find(field_names.begin(), field_names.end(), "c") != field_names.end();
    bool has_excluded1 =
        std::find(field_names.begin(), field_names.end(), "excluded1") != field_names.end();
    bool has_excluded2 =
        std::find(field_names.begin(), field_names.end(), "excluded2") != field_names.end();

    GOC_TEST_ASSERT(has_a, "Expected to find field 'a'");
    GOC_TEST_ASSERT(has_b, "Expected to find field 'b'");
    GOC_TEST_ASSERT(has_c, "Expected to find field 'c'");
    GOC_TEST_ASSERT(!has_excluded1, "Should not find field 'excluded1'");
    GOC_TEST_ASSERT(!has_excluded2, "Should not find field 'excluded2'");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseExclusion_Unterminated)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/unterminated.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class (Test), found %d", classes.size());

    const Ref<Class>& test_class = classes[0];
    Vector<Ref<Field>> fields = test_class->find_children<Field>(true);
    GOC_TEST_ASSERT(fields.size() == 1, "Expected 1 field (a), found %d", fields.size());

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseExclusion_NestedClassExclusion)
{
    GOC_TEST_PARSE_FILE("tests/files/exclusion_tests/nested_class_exclusion.h");

    Vector<Ref<Class>> classes = global_namespace->find_children<Class>(true);
    GOC_TEST_ASSERT(classes.size() == 1, "Expected 1 class (MyClass), found %d", classes.size());

    const Ref<Class>& my_class = classes[0];
    Vector<Ref<Field>> members = my_class->find_children<Field>(true);
    GOC_TEST_ASSERT(members.size() == 2, "Expected 2 members (a, b), found %d", members.size());

    return TEST_RESULT_SUCCESS;
};

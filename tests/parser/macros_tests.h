/**************************************************************************/
/* macros_tests.h                                                         */
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
#include "library/tree/syntax/namespace.h"
#include "library_godot/attributes/godot_attributes.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_TEST(ParseMacros)
{
    GOC_TEST_PARSE_FILE("tests/files/macro_tests/macros_no_args.h");

    const Ref<GodotClassAttribute> class_attribute =
        global_namespace->find_descendant<GodotClassAttribute>();
    GOC_TEST_ASSERT(class_attribute, "Failed to find class attribute");

    const Ref<GodotGeneratedBodyAttribute> generate_body_attribute =
        global_namespace->find_descendant<GodotGeneratedBodyAttribute>();
    GOC_TEST_ASSERT(generate_body_attribute, "Failed to find generated body attribute");

    const Ref<GodotPropertyCategoryAttribute> godot_property_category_attribute =
        global_namespace->find_descendant<GodotPropertyCategoryAttribute>();
    GOC_TEST_ASSERT(godot_property_category_attribute, "Failed to find category attribute");

    const Ref<GodotPropertyGroupAttribute> godot_property_group_attribute =
        global_namespace->find_descendant<GodotPropertyGroupAttribute>();
    GOC_TEST_ASSERT(godot_property_group_attribute, "Failed to find group attribute");

    const Ref<GodotPropertySubgroupAttribute> godot_property_subgroup_attribute =
        global_namespace->find_descendant<GodotPropertySubgroupAttribute>();
    GOC_TEST_ASSERT(godot_property_subgroup_attribute, "Failed to find subgroup attribute");

    const Ref<GodotPropertyAttribute> godot_property_attribute =
        global_namespace->find_descendant<GodotPropertyAttribute>();
    GOC_TEST_ASSERT(godot_property_attribute, "Failed to find property attribute");

    const Ref<GodotFunctionAttribute> godot_function_attribute =
        global_namespace->find_descendant<GodotFunctionAttribute>();
    GOC_TEST_ASSERT(godot_function_attribute, "Failed to find function attribute");

    const Ref<GodotSignalAttribute> godot_signal_attribute =
        global_namespace->find_descendant<GodotSignalAttribute>();
    GOC_TEST_ASSERT(godot_signal_attribute, "Failed to find signal attribute");

    const Ref<GodotEnumAttribute> godot_enum_attribute =
        global_namespace->find_descendant<GodotEnumAttribute>();
    GOC_TEST_ASSERT(godot_enum_attribute, "Failed to find enum attribute");

    const Ref<GodotGeneratedGlobalAttribute> godot_generated_global_attribute =
        global_namespace->find_descendant<GodotGeneratedGlobalAttribute>();
    GOC_TEST_ASSERT(godot_generated_global_attribute, "Failed to find generated global attribute");

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseDefine)
{
    GOC_TEST_PARSE_FILE("tests/files/macro_tests/define.h");

    auto defines = global_namespace->find_children<Define>(true);
    GOC_TEST_EQ(defines.size(), 2, "Invalid define count.");
    GOC_TEST_EQ(defines[0]->name(), "TEST_DEFINE", "Invalid test define name.");
    GOC_TEST_EQ(defines[1]->name(), "TEST_FUNC_DEFINE", "Invalid test function define name.");

    return TEST_RESULT_SUCCESS;
};
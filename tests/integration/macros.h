/**************************************************************************/
/* macros.h                                                               */
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
#include "library/tree_sitter_parser/parser.h"
#include "library_godot/attributes/godot_attributes.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(ParseMacrosWithArguments)
{
    GOC_TEST_PARSE_FILE("tests/files/integration_tests/macros.h");

    Ref<GodotClassAttribute> class_attribute =
        global_namespace->find_descendant<GodotClassAttribute>();
    GOC_TEST_ASSERT(class_attribute, "Failed to find class attribute");

    Ref<GodotClassTypeArgument> class_type_arg =
        class_attribute->find_descendant<GodotClassTypeArgument>();
    GOC_TEST_ASSERT(class_type_arg, "Failed to find class type argument");

    Ref<GodotModuleInitializationLevelArgument> initialization_level_argument =
        class_attribute->find_descendant<GodotModuleInitializationLevelArgument>();
    GOC_TEST_ASSERT(initialization_level_argument, "Failed to find initialization level argument");

    Ref<GodotGeneratedBodyAttribute> generate_body_attribute =
        global_namespace->find_descendant<GodotGeneratedBodyAttribute>();
    GOC_TEST_ASSERT(generate_body_attribute, "Failed to find generated body attribute");

    Ref<GodotPropertyCategoryAttribute> godot_property_category_attribute =
        global_namespace->find_descendant<GodotPropertyCategoryAttribute>();
    GOC_TEST_ASSERT(godot_property_category_attribute, "Failed to find category attribute");

    Ref<StringLiteralArgument> godot_property_category_literal_argument =
        godot_property_category_attribute->find_descendant<StringLiteralArgument>();
    GOC_TEST_ASSERT(
        godot_property_category_literal_argument,
        "Failed to find property category literal argument");

    Ref<GodotPropertyGroupAttribute> godot_property_group_attribute =
        global_namespace->find_descendant<GodotPropertyGroupAttribute>();
    GOC_TEST_ASSERT(godot_property_group_attribute, "Failed to find group attribute");

    Ref<StringLiteralArgument> godot_property_group_literal_argument =
        godot_property_group_attribute->find_descendant<StringLiteralArgument>();
    GOC_TEST_ASSERT(
        godot_property_group_literal_argument, "Failed to find property group literal argument");

    Ref<GodotPropertySubgroupAttribute> godot_property_subgroup_attribute =
        global_namespace->find_descendant<GodotPropertySubgroupAttribute>();
    GOC_TEST_ASSERT(godot_property_subgroup_attribute, "Failed to find subgroup attribute");

    Ref<StringLiteralArgument> godot_property_subgroup_literal_argument =
        godot_property_subgroup_attribute->find_descendant<StringLiteralArgument>();
    GOC_TEST_ASSERT(
        godot_property_subgroup_literal_argument,
        "Failed to find property subgroup literal argument");

    Ref<GodotPropertyAttribute> godot_property_attribute =
        global_namespace->find_descendant<GodotPropertyAttribute>();
    GOC_TEST_ASSERT(godot_property_attribute, "Failed to find property attribute");

    Ref<GodotVariantTypeArgument> godot_variant_type_argument =
        godot_property_attribute->find_descendant<GodotVariantTypeArgument>();
    GOC_TEST_ASSERT(godot_variant_type_argument, "Failed to find variant type argument");

    Ref<GodotPropertyHintArgument> godot_property_hint_argument =
        godot_property_attribute->find_descendant<GodotPropertyHintArgument>();
    GOC_TEST_ASSERT(godot_property_hint_argument, "Failed to find property hint");

    Ref<GodotPropertyUsageFlagsArgument> godot_property_usage_flags_argument =
        godot_property_attribute->find_descendant<GodotPropertyUsageFlagsArgument>();
    GOC_TEST_ASSERT(godot_property_usage_flags_argument, "Failed to find property usage flags");

    Ref<PropertyGetAccessSpecifierArgument> property_get_access_specifier_argument =
        godot_property_attribute->find_descendant<PropertyGetAccessSpecifierArgument>();
    GOC_TEST_ASSERT(
        property_get_access_specifier_argument, "Failed to find property access specifier");

    Ref<PropertySetAccessSpecifierArgument> property_set_access_specifier_argument =
        godot_property_attribute->find_descendant<PropertySetAccessSpecifierArgument>();
    GOC_TEST_ASSERT(
        property_set_access_specifier_argument, "Failed to find property access specifier");

    Ref<GodotFunctionAttribute> godot_function_attribute =
        global_namespace->find_descendant<GodotFunctionAttribute>();
    GOC_TEST_ASSERT(godot_function_attribute, "Failed to find function attribute");

    Ref<GodotVirtualArgument> godot_virtual_argument =
        godot_function_attribute->find_descendant<GodotVirtualArgument>();
    GOC_TEST_ASSERT(godot_virtual_argument, "Failed to find virtual argument");

    Ref<GodotRpcModeArgument> godot_rpc_mode_argument =
        godot_function_attribute->find_descendant<GodotRpcModeArgument>();
    GOC_TEST_ASSERT(godot_rpc_mode_argument, "Failed to find rpc mode argument");

    Ref<GodotRpcTransferModeArgument> godot_rpc_transfer_mode_argument =
        godot_function_attribute->find_descendant<GodotRpcTransferModeArgument>();
    GOC_TEST_ASSERT(godot_rpc_transfer_mode_argument, "Failed to find rpc transfer mode argument");

    Ref<GodotRpcSyncArgument> godot_rpc_sync_argument =
        godot_function_attribute->find_descendant<GodotRpcSyncArgument>();
    GOC_TEST_ASSERT(godot_rpc_sync_argument, "Failed to find rpc sync argument");

    Ref<GodotRpcChannelArgument> godot_rpc_channel_argument =
        godot_function_attribute->find_descendant<GodotRpcChannelArgument>();
    GOC_TEST_ASSERT(godot_rpc_channel_argument, "Failed to find rpc channel argument");

    Ref<GodotSignalAttribute> godot_signal_attribute =
        global_namespace->find_descendant<GodotSignalAttribute>();
    GOC_TEST_ASSERT(godot_signal_attribute, "Failed to find signal attribute");

    Ref<GodotEnumAttribute> godot_enum_attribute =
        global_namespace->find_descendant<GodotEnumAttribute>();
    GOC_TEST_ASSERT(godot_enum_attribute, "Failed to find enum attribute");

    Ref<EnumGeneratorOptionsArgument> generator_options_argument =
        godot_enum_attribute->find_descendant<EnumGeneratorOptionsArgument>();
    GOC_TEST_ASSERT(generator_options_argument, "Failed to find enum generator options");

    Ref<GodotGeneratedGlobalAttribute> godot_generated_global_attribute =
        global_namespace->find_descendant<GodotGeneratedGlobalAttribute>();
    GOC_TEST_ASSERT(godot_generated_global_attribute, "Failed to find generated global attribute");

    return TEST_RESULT_SUCCESS;
};
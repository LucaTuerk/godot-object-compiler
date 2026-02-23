/**************************************************************************/
/* properties.h                                                           */
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
#include "common.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/namespace.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(GeneratePropertyBinds) {
	GOC_INTEGRATION_TEST_GEN_FILE("tests/files/integration_tests/properties.h");

	GOC_ASSERT_PROP_BOUND("aabb_property", "AABB");
	GOC_ASSERT_PROP_BOUND("array_property", "ARRAY");
	GOC_ASSERT_PROP_BOUND("typed_array_property", "ARRAY");
	GOC_ASSERT_PROP_BOUND("basis_property", "BASIS");
	GOC_ASSERT_PROP_BOUND("bool_property", "BOOL");
	GOC_ASSERT_PROP_BOUND("callable_property", "CALLABLE");
	GOC_ASSERT_PROP_BOUND("color_property", "COLOR");
	GOC_ASSERT_PROP_BOUND("dictionary_property", "DICTIONARY");
	GOC_ASSERT_PROP_BOUND("typed_dictionary_property", "DICTIONARY");
	GOC_ASSERT_PROP_BOUND("float_property", "FLOAT");
	GOC_ASSERT_PROP_BOUND("real_t_property", "FLOAT");
	GOC_ASSERT_PROP_BOUND("double_property", "FLOAT");
	GOC_ASSERT_PROP_BOUND("int_property", "INT");
	GOC_ASSERT_PROP_BOUND("long_property", "INT");
	GOC_ASSERT_PROP_BOUND("node_path_property", "NODE_PATH");
	GOC_ASSERT_PROP_BOUND("object_property", "OBJECT");
	GOC_ASSERT_PROP_BOUND("packed_byte_array_property", "PACKED_BYTE_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_color_array_property", "PACKED_COLOR_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_float32_array_property", "PACKED_FLOAT32_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_float64_array_property", "PACKED_FLOAT64_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_int32_array_property", "PACKED_INT32_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_int64_array_property", "PACKED_INT64_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_string_array_property", "PACKED_STRING_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_vector2_array_property", "PACKED_VECTOR2_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_vector3_array_property", "PACKED_VECTOR3_ARRAY");
	GOC_ASSERT_PROP_BOUND("packed_vector4_array_property", "PACKED_VECTOR4_ARRAY");
	GOC_ASSERT_PROP_BOUND("plane_property", "PLANE");
	GOC_ASSERT_PROP_BOUND("projection_property", "PROJECTION");
	GOC_ASSERT_PROP_BOUND("quaternion_property", "QUATERNION");
	GOC_ASSERT_PROP_BOUND("rect2_property", "RECT2");
	GOC_ASSERT_PROP_BOUND("rect2i_property", "RECT2I");
	GOC_ASSERT_PROP_BOUND("signal_property", "SIGNAL");
	GOC_ASSERT_PROP_BOUND("string_property", "STRING");
	GOC_ASSERT_PROP_BOUND("string_name_property", "STRING_NAME");
	GOC_ASSERT_PROP_BOUND("transform2D_property", "TRANSFORM2D");
	GOC_ASSERT_PROP_BOUND("transform3D_property", "TRANSFORM3D");
	GOC_ASSERT_PROP_BOUND("vector2_property", "VECTOR2");
	GOC_ASSERT_PROP_BOUND("vector2i_property", "VECTOR2I");
	GOC_ASSERT_PROP_BOUND("vector3_property", "VECTOR3");
	GOC_ASSERT_PROP_BOUND("vector3i_property", "VECTOR3I");
	GOC_ASSERT_PROP_BOUND("vector4_property", "VECTOR4");
	GOC_ASSERT_PROP_BOUND("vector4i_property", "VECTOR4I");
	GOC_ASSERT_PROP_BOUND("node_property", "OBJECT");
	GOC_ASSERT_PROP_BOUND("resource_property", "OBJECT");
	GOC_ASSERT_PROP_BOUND("enum_property", "INT");

	return TEST_RESULT_SUCCESS;
}
/**************************************************************************/
/* virtual_functions.h                                                    */
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

GOC_INTEGRATION_TEST(GenerateVirtualFunctionBinds) {
	GOC_INTEGRATION_TEST_GEN_FILE("tests/files/integration_tests/virtual_functions.h");

	GOC_ASSERT_QUALIFIED_NAME("Tests::VirtualFunctions", generated_source);
	GOC_ASSERT_FUNC_BOUND("aabb_function");
	GOC_ASSERT_VIRTUAL_BOUND("aabb_function", "AABB");
	GOC_ASSERT_FUNC_BOUND("array_function");
	GOC_ASSERT_VIRTUAL_BOUND("array_function", "Array");
	GOC_ASSERT_FUNC_BOUND("typed_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("typed_array_function", "TypedArray");
	GOC_ASSERT_FUNC_BOUND("basis_function");
	GOC_ASSERT_VIRTUAL_BOUND("basis_function", "Basis");
	GOC_ASSERT_FUNC_BOUND("bool_function");
	GOC_ASSERT_VIRTUAL_BOUND("bool_function", "bool");
	GOC_ASSERT_FUNC_BOUND("callable_function");
	GOC_ASSERT_VIRTUAL_BOUND("callable_function", "Callable");
	GOC_ASSERT_FUNC_BOUND("color_function");
	GOC_ASSERT_VIRTUAL_BOUND("color_function", "Color");
	GOC_ASSERT_FUNC_BOUND("dictionary_function");
	GOC_ASSERT_VIRTUAL_BOUND("dictionary_function", "Dictionary");
	GOC_ASSERT_FUNC_BOUND("typed_dictionary_function");
	GOC_ASSERT_VIRTUAL_BOUND("typed_dictionary_function", "TypedDictionary");
	GOC_ASSERT_FUNC_BOUND("float_function");
	GOC_ASSERT_VIRTUAL_BOUND("float_function", "float");
	GOC_ASSERT_FUNC_BOUND("real_t_function");
	GOC_ASSERT_VIRTUAL_BOUND("real_t_function", "real_t");
	GOC_ASSERT_FUNC_BOUND("double_function");
	GOC_ASSERT_VIRTUAL_BOUND("double_function", "double");
	GOC_ASSERT_FUNC_BOUND("int_function");
	GOC_ASSERT_VIRTUAL_BOUND("int_function", "int");
	GOC_ASSERT_FUNC_BOUND("long_function");
	GOC_ASSERT_VIRTUAL_BOUND("long_function", "long");
	GOC_ASSERT_FUNC_BOUND("node_path_function");
	GOC_ASSERT_VIRTUAL_BOUND("node_path_function", "NodePath");
	GOC_ASSERT_FUNC_BOUND("object_function");
	GOC_ASSERT_VIRTUAL_BOUND("object_function", "Object");
	GOC_ASSERT_FUNC_BOUND("packed_byte_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_byte_array_function", "PackedByteArray");
	GOC_ASSERT_FUNC_BOUND("packed_color_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_color_array_function", "PackedColorArray");
	GOC_ASSERT_FUNC_BOUND("packed_float32_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_float32_array_function", "PackedFloat32Array");
	GOC_ASSERT_FUNC_BOUND("packed_float64_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_float64_array_function", "PackedFloat64Array");
	GOC_ASSERT_FUNC_BOUND("packed_int32_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_int32_array_function", "PackedInt32Array");
	GOC_ASSERT_FUNC_BOUND("packed_int64_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_int64_array_function", "PackedInt64Array");
	GOC_ASSERT_FUNC_BOUND("packed_string_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_string_array_function", "PackedStringArray");
	GOC_ASSERT_FUNC_BOUND("packed_vector2_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_vector2_array_function", "PackedVector2Array");
	GOC_ASSERT_FUNC_BOUND("packed_vector3_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_vector3_array_function", "PackedVector3Array");
	GOC_ASSERT_FUNC_BOUND("packed_vector4_array_function");
	GOC_ASSERT_VIRTUAL_BOUND("packed_vector4_array_function", "PackedVector4Array");
	GOC_ASSERT_FUNC_BOUND("plane_function");
	GOC_ASSERT_VIRTUAL_BOUND("plane_function", "Plane");
	GOC_ASSERT_FUNC_BOUND("projection_function");
	GOC_ASSERT_VIRTUAL_BOUND("projection_function", "Projection");
	GOC_ASSERT_FUNC_BOUND("quaternion_function");
	GOC_ASSERT_VIRTUAL_BOUND("quaternion_function", "Quaternion");
	GOC_ASSERT_FUNC_BOUND("rect2_function");
	GOC_ASSERT_VIRTUAL_BOUND("rect2_function", "Rect2");
	GOC_ASSERT_FUNC_BOUND("rect2i_function");
	GOC_ASSERT_VIRTUAL_BOUND("rect2i_function", "Rect2i");
	GOC_ASSERT_FUNC_BOUND("signal_function");
	GOC_ASSERT_VIRTUAL_BOUND("signal_function", "Signal");
	GOC_ASSERT_FUNC_BOUND("string_function");
	GOC_ASSERT_VIRTUAL_BOUND("string_function", "String");
	GOC_ASSERT_FUNC_BOUND("string_name_function");
	GOC_ASSERT_VIRTUAL_BOUND("string_name_function", "StringName");
	GOC_ASSERT_FUNC_BOUND("transform2D_function");
	GOC_ASSERT_VIRTUAL_BOUND("transform2D_function", "Transform2D");
	GOC_ASSERT_FUNC_BOUND("transform3D_function");
	GOC_ASSERT_VIRTUAL_BOUND("transform3D_function", "Transform3D");
	GOC_ASSERT_FUNC_BOUND("vector2_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector2_function", "Vector2");
	GOC_ASSERT_FUNC_BOUND("vector2i_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector2i_function", "Vector2i");
	GOC_ASSERT_FUNC_BOUND("vector3_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector3_function", "Vector3");
	GOC_ASSERT_FUNC_BOUND("vector3i_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector3i_function", "Vector3i");
	GOC_ASSERT_FUNC_BOUND("vector4_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector4_function", "Vector4");
	GOC_ASSERT_FUNC_BOUND("vector4i_function");
	GOC_ASSERT_VIRTUAL_BOUND("vector4i_function", "Vector4i");
	GOC_ASSERT_FUNC_BOUND("node_function");
	GOC_ASSERT_VIRTUAL_BOUND("node_function", "Node");
	GOC_ASSERT_FUNC_BOUND("resource_function");
	GOC_ASSERT_VIRTUAL_BOUND("resource_function", "Resource");

	return TEST_RESULT_SUCCESS;
};
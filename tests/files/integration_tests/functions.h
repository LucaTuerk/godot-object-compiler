/**************************************************************************/
/* functions.h                                                            */
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

namespace Tests {
GODOT_CLASS();

class Functions : Node {
	GODOT_GENERATED_BODY();

	GODOT_FUNCTION();
	AABB aabb_function();

	GODOT_FUNCTION();
	Array array_function();

	GODOT_FUNCTION();
	TypedArray<Resource> typed_array_function();

	GODOT_FUNCTION();
	Basis basis_function();

	GODOT_FUNCTION();
	bool bool_function();

	GODOT_FUNCTION();
	Callable callable_function();

	GODOT_FUNCTION();
	Color color_function();

	GODOT_FUNCTION();
	Dictionary dictionary_function();

	GODOT_FUNCTION();
	TypedDictionary<int, Resource> typed_dictionary_function();

	GODOT_FUNCTION();
	float float_function();

	GODOT_FUNCTION();
	real_t real_t_function();

	GODOT_FUNCTION();
	double double_function();

	GODOT_FUNCTION();
	int int_function();

	GODOT_FUNCTION();
	long long_function();

	GODOT_FUNCTION();
	NodePath node_path_function();

	GODOT_FUNCTION();
	Object *object_function();

	GODOT_FUNCTION();
	PackedByteArray packed_byte_array_function();

	GODOT_FUNCTION();
	PackedColorArray packed_color_array_function();

	GODOT_FUNCTION();
	PackedFloat32Array packed_float32_array_function();

	GODOT_FUNCTION();
	PackedFloat64Array packed_float64_array_function();

	GODOT_FUNCTION();
	PackedInt32Array packed_int32_array_function();

	GODOT_FUNCTION();
	PackedInt64Array packed_int64_array_function();

	GODOT_FUNCTION();
	PackedStringArray packed_string_array_function();

	GODOT_FUNCTION();
	PackedVector2Array packed_vector2_array_function();

	GODOT_FUNCTION();
	PackedVector3Array packed_vector3_array_function();

	GODOT_FUNCTION();
	PackedVector4Array packed_vector4_array_function();

	GODOT_FUNCTION();
	Plane plane_function();

	GODOT_FUNCTION();
	Projection projection_function();

	GODOT_FUNCTION();
	Quaternion quaternion_function();

	GODOT_FUNCTION();
	Rect2 rect2_function();

	GODOT_FUNCTION();
	Rect2i rect2i_function();

	GODOT_FUNCTION();
	Signal signal_function();

	GODOT_FUNCTION();
	String string_function();

	GODOT_FUNCTION();
	StringName string_name_function();

	GODOT_FUNCTION();
	Transform2D transform2D_function();

	GODOT_FUNCTION();
	Transform3D transform3D_function();

	GODOT_FUNCTION();
	Vector2 vector2_function();

	GODOT_FUNCTION();
	Vector2i vector2i_function();

	GODOT_FUNCTION();
	Vector3 vector3_function();

	GODOT_FUNCTION();
	Vector3i vector3i_function();

	GODOT_FUNCTION();
	Vector4 vector4_function();

	GODOT_FUNCTION();
	Vector4i vector4i_function();

	GODOT_FUNCTION();
	Node *node_function();

	GODOT_FUNCTION();
	Ref<Resource> resource_function();
};
} //namespace Tests

GODOT_GENERATED_GLOBAL();
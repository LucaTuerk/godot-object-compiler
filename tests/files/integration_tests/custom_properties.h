/**************************************************************************/
/* custom_properties.h                                                    */
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

#include "custom_properties.generated.h"

namespace Tests {
GODOT_CLASS();

class A : public Node {
	GODOT_GENERATED_BODY();

	GODOT_PROPERTY("aabb_property", GetSet("get_aabb_property", "set_aabb_property"));
	AABB get_aabb_property() const;
	void set_aabb_property(const AABB &p_value);

	GODOT_PROPERTY("array_property", GetSet("get_array_property", "set_array_property"));
	Array get_array_property() const;
	void set_array_property(const Array &p_value);

	GODOT_PROPERTY(
			"typed_array_property", GetSet("get_typed_array_property", "set_typed_array_property"));
	TypedArray<Resource> get_typed_array_property() const;
	void set_typed_array_property(const TypedArray<Resource> &p_value);

	GODOT_PROPERTY("basis_property", GetSet("get_basis_property", "set_basis_property"));
	Basis get_basis_property() const;
	void set_basis_property(const Basis &p_value);

	GODOT_PROPERTY("bool_property", GetSet("get_bool_property", "set_bool_property"));
	bool get_bool_property() const;
	void set_bool_property(const bool &p_value);

	GODOT_PROPERTY("callable_property", GetSet("get_callable_property", "set_callable_property"));
	Callable get_callable_property() const;
	void set_callable_property(const Callable &p_value);

	GODOT_PROPERTY("color_property", GetSet("get_color_property", "set_color_property"));
	Color get_color_property() const;
	void set_color_property(const Color &p_value);

	GODOT_PROPERTY(
			"dictionary_property", GetSet("get_dictionary_property", "set_dictionary_property"));
	Dictionary get_dictionary_property() const;
	void set_dictionary_property(const Dictionary &p_value);

	GODOT_PROPERTY(
			"typed_dictionary_property",
			GetSet("get_typed_dictionary_property", "set_typed_dictionary_property"));
	TypedDictionary<int, Resource> get_typed_dictionary_property() const;
	void set_typed_dictionary_property(const TypedDictionary<int, Resource> &p_value);

	GODOT_PROPERTY("float_property", GetSet("get_float_property", "set_float_property"));
	float get_float_property() const;
	void set_float_property(const float &p_value);

	GODOT_PROPERTY("real_t_property", GetSet("get_real_t_property", "set_real_t_property"));
	real_t get_real_t_property() const;
	void set_real_t_property(const real_t &p_value);

	GODOT_PROPERTY("double_property", GetSet("get_double_property", "set_double_property"));
	double get_double_property() const;
	void set_double_property(const double &p_value);

	GODOT_PROPERTY("int_property", GetSet("get_int_property", "set_int_property"));
	int get_int_property() const;
	void set_int_property(const int &p_value);

	GODOT_PROPERTY("long_property", GetSet("get_long_property", "set_long_property"));
	long get_long_property() const;
	void set_long_property(const long &p_value);

	GODOT_PROPERTY(
			"node_path_property", GetSet("get_node_path_property", "set_node_path_property"));
	NodePath get_node_path_property() const;
	void set_node_path_property(const NodePath &p_value);

	GODOT_PROPERTY("object_property", GetSet("get_object_property", "set_object_property"));
	Object *get_object_property() const;
	void set_object_property(const Object *&p_value);

	GODOT_PROPERTY(
			"packed_byte_array_property",
			GetSet("get_packed_byte_array_property", "set_packed_byte_array_property"));
	PackedByteArray get_packed_byte_array_property() const;
	void set_packed_byte_array_property(const PackedByteArray &p_value);

	GODOT_PROPERTY(
			"packed_color_array_property",
			GetSet("get_packed_color_array_property", "set_packed_color_array_property"));
	PackedColorArray get_packed_color_array_property() const;
	void set_packed_color_array_property(const PackedColorArray &p_value);

	GODOT_PROPERTY(
			"packed_float32_array_property",
			GetSet("get_packed_float32_array_property", "set_packed_float32_array_property"));
	PackedFloat32Array get_packed_float32_array_property() const;
	void set_packed_float32_array_property(const PackedFloat32Array &p_value);

	GODOT_PROPERTY(
			"packed_float64_array_property",
			GetSet("get_packed_float64_array_property", "set_packed_float64_array_property"));
	PackedFloat64Array get_packed_float64_array_property() const;
	void set_packed_float64_array_property(const PackedFloat64Array &p_value);

	GODOT_PROPERTY(
			"packed_int32_array_property",
			GetSet("get_packed_int32_array_property", "set_packed_int32_array_property"));
	PackedInt32Array get_packed_int32_array_property() const;
	void set_packed_int32_array_property(const PackedInt32Array &p_value);

	GODOT_PROPERTY(
			"packed_int64_array_property",
			GetSet("get_packed_int64_array_property", "set_packed_int64_array_property"));
	PackedInt64Array get_packed_int64_array_property() const;
	void set_packed_int64_array_property(const PackedInt64Array &p_value);

	GODOT_PROPERTY(
			"packed_string_array_property",
			GetSet("get_packed_string_array_property", "set_packed_string_array_property"));
	PackedStringArray get_packed_string_array_property() const;
	void set_packed_string_array_property(const PackedStringArray &p_value);

	GODOT_PROPERTY(
			"packed_vector2_array_property",
			GetSet("get_packed_vector2_array_property", "set_packed_vector2_array_property"));
	PackedVector2Array get_packed_vector2_array_property() const;
	void set_packed_vector2_array_property(const PackedVector2Array &p_value);

	GODOT_PROPERTY(
			"packed_vector3_array_property",
			GetSet("get_packed_vector3_array_property", "set_packed_vector3_array_property"));
	PackedVector3Array get_packed_vector3_array_property() const;
	void set_packed_vector3_array_property(const PackedVector3Array &p_value);

	GODOT_PROPERTY(
			"packed_vector4_array_property",
			GetSet("get_packed_vector4_array_property", "set_packed_vector4_array_property"));
	PackedVector4Array get_packed_vector4_array_property() const;
	void set_packed_vector4_array_property(const PackedVector4Array &p_value);

	GODOT_PROPERTY("plane_property", GetSet("get_plane_property", "set_plane_property"));
	Plane get_plane_property() const;
	void set_plane_property(const Plane &p_value);

	GODOT_PROPERTY(
			"projection_property", GetSet("get_projection_property", "set_projection_property"));
	Projection get_projection_property() const;
	void set_projection_property(const Projection &p_value);

	GODOT_PROPERTY(
			"quaternion_property", GetSet("get_quaternion_property", "set_quaternion_property"));
	Quaternion get_quaternion_property() const;
	void set_quaternion_property(const Quaternion &p_value);

	GODOT_PROPERTY("rect2_property", GetSet("get_rect2_property", "set_rect2_property"));
	Rect2 get_rect2_property() const;
	void set_rect2_property(const Rect2 &p_value);

	GODOT_PROPERTY("rect2i_property", GetSet("get_rect2i_property", "set_rect2i_property"));
	Rect2i get_rect2i_property() const;
	void set_rect2i_property(const Rect2i &p_value);

	GODOT_PROPERTY("signal_property", GetSet("get_signal_property", "set_signal_property"));
	Signal get_signal_property() const;
	void set_signal_property(const Signal &p_value);

	GODOT_PROPERTY("string_property", GetSet("get_string_property", "set_string_property"));
	String get_string_property() const;
	void set_string_property(const String &p_value);

	GODOT_PROPERTY(
			"string_name_property", GetSet("get_string_name_property", "set_string_name_property"));
	StringName get_string_name_property() const;
	void set_string_name_property(const StringName &p_value);

	GODOT_PROPERTY(
			"transform2D_property", GetSet("get_transform2D_property", "set_transform2D_property"));
	Transform2D get_transform2D_property() const;
	void set_transform2D_property(const Transform2D &p_value);

	GODOT_PROPERTY(
			"transform3D_property", GetSet("get_transform3D_property", "set_transform3D_property"));
	Transform3D get_transform3D_property() const;
	void set_transform3D_property(const Transform3D &p_value);

	GODOT_PROPERTY("vector2_property", GetSet("get_vector2_property", "set_vector2_property"));
	Vector2 get_vector2_property() const;
	void set_vector2_property(const Vector2 &p_value);

	GODOT_PROPERTY("vector2i_property", GetSet("get_vector2i_property", "set_vector2i_property"));
	Vector2i get_vector2i_property() const;
	void set_vector2i_property(const Vector2i &p_value);

	GODOT_PROPERTY("vector3_property", GetSet("get_vector3_property", "set_vector3_property"));
	Vector3 get_vector3_property() const;
	void set_vector3_property(const Vector3 &p_value);

	GODOT_PROPERTY("vector3i_property", GetSet("get_vector3i_property", "set_vector3i_property"));
	Vector3i get_vector3i_property() const;
	void set_vector3i_property(const Vector3i &p_value);

	GODOT_PROPERTY("vector4_property", GetSet("get_vector4_property", "set_vector4_property"));
	Vector4 get_vector4_property() const;
	void set_vector4_property(const Vector4 &p_value);

	GODOT_PROPERTY("vector4i_property", GetSet("get_vector4i_property", "set_vector4i_property"));
	Vector4i get_vector4i_property() const;
	void set_vector4i_property(const Vector4i &p_value);

	GODOT_PROPERTY("node_property", GetSet("get_node_property", "set_node_property"));
	Node *get_node_property() const;
	void set_node_property(Node *p_value);

	GODOT_PROPERTY("resource_property", GetSet("get_resource_property", "set_resource_property"));
	Ref<Resource> get_resource_property() const;
	void set_resource_property(const Ref<Resource> &p_value);

	GODOT_ENUM();

	enum Enum { ENUM_VALUE };

	GODOT_PROPERTY("enum_property", GetSet("get_enum_property", "set_enum_property"));
	Enum get_enum_property() const;
	void set_enum_property(const Enum &p_value);
};
} // namespace Tests

GODOT_GENERATED_GLOBAL();
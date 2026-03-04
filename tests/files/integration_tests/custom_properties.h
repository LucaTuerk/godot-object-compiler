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

namespace Tests {
GODOT_CLASS();

class A : public Node {
	GODOT_GENERATED_BODY();

	GODOT_PROPERTY(Name("aabb_property"), Get("get_aabb_property"), Set("set_aabb_property"));
	AABB get_aabb_property() const;
	void set_aabb_property(const AABB &p_value);

	GODOT_PROPERTY(Name("array_property"), Get("get_array_property"), Set("set_array_property"));
	Array get_array_property() const;
	void set_array_property(const Array &p_value);

	GODOT_PROPERTY(Name("typed_array_property"), Get("get_typed_array_property"), Set("set_typed_array_property"));
	TypedArray<Resource> get_typed_array_property() const;
	void set_typed_array_property(const TypedArray<Resource> &p_value);

	GODOT_PROPERTY(Name("basis_property"), Get("get_basis_property"), Set("set_basis_property"));
	Basis get_basis_property() const;
	void set_basis_property(const Basis &p_value);

	GODOT_PROPERTY(Name("bool_property"), Get("get_bool_property"), Set("set_bool_property"));
	bool get_bool_property() const;
	void set_bool_property(const bool &p_value);

	GODOT_PROPERTY(Name("callable_property"), Get("get_callable_property"), Set("set_callable_property"));
	Callable get_callable_property() const;
	void set_callable_property(const Callable &p_value);

	GODOT_PROPERTY(Name("color_property"), Get("get_color_property"), Set("set_color_property"));
	Color get_color_property() const;
	void set_color_property(const Color &p_value);

	GODOT_PROPERTY(Name("dictionary_property"), Get("get_dictionary_property"), Set("set_dictionary_property"));
	Dictionary get_dictionary_property() const;
	void set_dictionary_property(const Dictionary &p_value);

	GODOT_PROPERTY(Name("typed_dictionary_property"), Get("get_typed_dictionary_property"), Set("set_typed_dictionary_property"));
	TypedDictionary<int, Resource> get_typed_dictionary_property() const;
	void set_typed_dictionary_property(const TypedDictionary<int, Resource> &p_value);

	GODOT_PROPERTY(Name("float_property"), Get("get_float_property"), Set("set_float_property"));
	float get_float_property() const;
	void set_float_property(const float &p_value);

	GODOT_PROPERTY(Name("real_t_property"), Get("get_real_t_property"), Set("set_real_t_property"));
	real_t get_real_t_property() const;
	void set_real_t_property(const real_t &p_value);

	GODOT_PROPERTY(Name("double_property"), Get("get_double_property"), Set("set_double_property"));
	double get_double_property() const;
	void set_double_property(const double &p_value);

	GODOT_PROPERTY(Name("int_property"), Get("get_int_property"), Set("set_int_property"));
	int get_int_property() const;
	void set_int_property(const int &p_value);

	GODOT_PROPERTY(Name("long_property"), Get("get_long_property"), Set("set_long_property"));
	long get_long_property() const;
	void set_long_property(const long &p_value);

	GODOT_PROPERTY(Name("node_path_property"), Get("get_node_path_property"), Set("set_node_path_property"));
	NodePath get_node_path_property() const;
	void set_node_path_property(const NodePath &p_value);

	GODOT_PROPERTY(Name("object_property"), Get("get_object_property"), Set("set_object_property"));
	Object *get_object_property() const;
	void set_object_property(const Object *&p_value);

	GODOT_PROPERTY(Name("packed_byte_array_property"), Get("get_packed_byte_array_property"), Set("set_packed_byte_array_property"));
	PackedByteArray get_packed_byte_array_property() const;
	void set_packed_byte_array_property(const PackedByteArray &p_value);

	GODOT_PROPERTY(Name("packed_color_array_property"), Get("get_packed_color_array_property"), Set("set_packed_color_array_property"));
	PackedColorArray get_packed_color_array_property() const;
	void set_packed_color_array_property(const PackedColorArray &p_value);

	GODOT_PROPERTY(Name("packed_float32_array_property"), Get("get_packed_float32_array_property"), Set("set_packed_float32_array_property"));
	PackedFloat32Array get_packed_float32_array_property() const;
	void set_packed_float32_array_property(const PackedFloat32Array &p_value);

	GODOT_PROPERTY(Name("packed_float64_array_property"), Get("get_packed_float64_array_property"), Set("set_packed_float64_array_property"));
	PackedFloat64Array get_packed_float64_array_property() const;
	void set_packed_float64_array_property(const PackedFloat64Array &p_value);

	GODOT_PROPERTY(Name("packed_int32_array_property"), Get("get_packed_int32_array_property"), Set("set_packed_int32_array_property"));
	PackedInt32Array get_packed_int32_array_property() const;
	void set_packed_int32_array_property(const PackedInt32Array &p_value);

	GODOT_PROPERTY(Name("packed_int64_array_property"), Get("get_packed_int64_array_property"), Set("set_packed_int64_array_property"));
	PackedInt64Array get_packed_int64_array_property() const;
	void set_packed_int64_array_property(const PackedInt64Array &p_value);

	GODOT_PROPERTY(Name("packed_string_array_property"), Get("get_packed_string_array_property"), Set("set_packed_string_array_property"));
	PackedStringArray get_packed_string_array_property() const;
	void set_packed_string_array_property(const PackedStringArray &p_value);

	GODOT_PROPERTY(Name("packed_vector2_array_property"), Get("get_packed_vector2_array_property"), Set("set_packed_vector2_array_property"));
	PackedVector2Array get_packed_vector2_array_property() const;
	void set_packed_vector2_array_property(const PackedVector2Array &p_value);

	GODOT_PROPERTY(Name("packed_vector3_array_property"), Get("get_packed_vector3_array_property"), Set("set_packed_vector3_array_property"));
	PackedVector3Array get_packed_vector3_array_property() const;
	void set_packed_vector3_array_property(const PackedVector3Array &p_value);

	GODOT_PROPERTY(Name("packed_vector4_array_property"), Get("get_packed_vector4_array_property"), Set("set_packed_vector4_array_property"));
	PackedVector4Array get_packed_vector4_array_property() const;
	void set_packed_vector4_array_property(const PackedVector4Array &p_value);

	GODOT_PROPERTY(Name("plane_property"), Get("get_plane_property"), Set("set_plane_property"));
	Plane get_plane_property() const;
	void set_plane_property(const Plane &p_value);

	GODOT_PROPERTY(Name("projection_property"), Get("get_projection_property"), Set("set_projection_property"));
	Projection get_projection_property() const;
	void set_projection_property(const Projection &p_value);

	GODOT_PROPERTY(Name("quaternion_property"), Get("get_quaternion_property"), Set("set_quaternion_property"));
	Quaternion get_quaternion_property() const;
	void set_quaternion_property(const Quaternion &p_value);

	GODOT_PROPERTY(Name("rect2_property"), Get("get_rect2_property"), Set("set_rect2_property"));
	Rect2 get_rect2_property() const;
	void set_rect2_property(const Rect2 &p_value);

	GODOT_PROPERTY(Name("rect2i_property"), Get("get_rect2i_property"), Set("set_rect2i_property"));
	Rect2i get_rect2i_property() const;
	void set_rect2i_property(const Rect2i &p_value);

	GODOT_PROPERTY(Name("signal_property"), Get("get_signal_property"), Set("set_signal_property"));
	Signal get_signal_property() const;
	void set_signal_property(const Signal &p_value);

	GODOT_PROPERTY(Name("string_property"), Get("get_string_property"), Set("set_string_property"));
	String get_string_property() const;
	void set_string_property(const String &p_value);

	GODOT_PROPERTY(Name("string_name_property"), Get("get_string_name_property"), Set("set_string_name_property"));
	StringName get_string_name_property() const;
	void set_string_name_property(const StringName &p_value);

	GODOT_PROPERTY(Name("transform2D_property"), Get("get_transform2D_property"), Set("set_transform2D_property"));
	Transform2D get_transform2D_property() const;
	void set_transform2D_property(const Transform2D &p_value);

	GODOT_PROPERTY(Name("transform3D_property"), Get("get_transform3D_property"), Set("set_transform3D_property"));
	Transform3D get_transform3D_property() const;
	void set_transform3D_property(const Transform3D &p_value);

	GODOT_PROPERTY(Name("vector2_property"), Get("get_vector2_property"), Set("set_vector2_property"));
	Vector2 get_vector2_property() const;
	void set_vector2_property(const Vector2 &p_value);

	GODOT_PROPERTY(Name("vector2i_property"), Get("get_vector2i_property"), Set("set_vector2i_property"));
	Vector2i get_vector2i_property() const;
	void set_vector2i_property(const Vector2i &p_value);

	GODOT_PROPERTY(Name("vector3_property"), Get("get_vector3_property"), Set("set_vector3_property"));
	Vector3 get_vector3_property() const;
	void set_vector3_property(const Vector3 &p_value);

	GODOT_PROPERTY(Name("vector3i_property"), Get("get_vector3i_property"), Set("set_vector3i_property"));
	Vector3i get_vector3i_property() const;
	void set_vector3i_property(const Vector3i &p_value);

	GODOT_PROPERTY(Name("vector4_property"), Get("get_vector4_property"), Set("set_vector4_property"));
	Vector4 get_vector4_property() const;
	void set_vector4_property(const Vector4 &p_value);

	GODOT_PROPERTY(Name("vector4i_property"), Get("get_vector4i_property"), Set("set_vector4i_property"));
	Vector4i get_vector4i_property() const;
	void set_vector4i_property(const Vector4i &p_value);

	GODOT_PROPERTY(Name("node_property"), Get("get_node_property"), Set("set_node_property"));
	Node *get_node_property() const;
	void set_node_property(Node *p_value);

	GODOT_PROPERTY(Name("resource_property"), Get("get_resource_property"), Set("set_resource_property"));
	Ref<Resource> get_resource_property() const;
	void set_resource_property(const Ref<Resource> &p_value);

	GODOT_ENUM();

	enum Enum { ENUM_VALUE };

	GODOT_PROPERTY(Name("enum_property"), Get("get_enum_property"), Set("set_enum_property"));
	Enum get_enum_property() const;
	void set_enum_property(const Enum &p_value);
};
} //namespace Tests

GODOT_GENERATED_GLOBAL();
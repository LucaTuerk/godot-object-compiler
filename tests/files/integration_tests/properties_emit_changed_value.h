/**************************************************************************/
/* properties_emit_changed_value.h                                        */
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

#include "properties_emit_changed_value.generated.h"

namespace Tests {
GODOT_CLASS();
class PropertiesEmitChangedValue : public Node {
  GODOT_GENERATED_BODY();

  GODOT_PROPERTY(EmitChangedValue());
  AABB aabb_property;

  GODOT_PROPERTY(EmitChangedValue());
  Array array_property;

  GODOT_PROPERTY(EmitChangedValue());
  TypedArray<Resource> typed_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  Basis basis_property;

  GODOT_PROPERTY(EmitChangedValue());
  bool bool_property = false;

  GODOT_PROPERTY(EmitChangedValue());
  Callable callable_property;

  GODOT_PROPERTY(EmitChangedValue());
  Color color_property;

  GODOT_PROPERTY(EmitChangedValue());
  Dictionary dictionary_property;

  GODOT_PROPERTY(EmitChangedValue());
  TypedDictionary<int, Resource> typed_dictionary_property;

  GODOT_PROPERTY(EmitChangedValue());
  float float_property = 0;

  GODOT_PROPERTY(EmitChangedValue());
  real_t real_t_property = 0;

  GODOT_PROPERTY(EmitChangedValue());
  double double_property = 0;

  GODOT_PROPERTY(EmitChangedValue());
  int int_property = 0;

  GODOT_PROPERTY(EmitChangedValue());
  long long_property = 0;

  GODOT_PROPERTY(EmitChangedValue());
  NodePath node_path_property;

  GODOT_PROPERTY(EmitChangedValue());
  Object* object_property = nullptr;

  GODOT_PROPERTY(EmitChangedValue());
  PackedByteArray packed_byte_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedColorArray packed_color_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedFloat32Array packed_float32_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedFloat64Array packed_float64_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedInt32Array packed_int32_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedInt64Array packed_int64_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedStringArray packed_string_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedVector2Array packed_vector2_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedVector3Array packed_vector3_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  PackedVector4Array packed_vector4_array_property;

  GODOT_PROPERTY(EmitChangedValue());
  Plane plane_property;

  GODOT_PROPERTY(EmitChangedValue());
  Projection projection_property;

  GODOT_PROPERTY(EmitChangedValue());
  Quaternion quaternion_property;

  GODOT_PROPERTY(EmitChangedValue());
  Rect2 rect2_property;

  GODOT_PROPERTY(EmitChangedValue());
  Rect2i rect2i_property;

  GODOT_PROPERTY(EmitChangedValue());
  Signal signal_property;

  GODOT_PROPERTY(EmitChangedValue());
  String string_property;

  GODOT_PROPERTY(EmitChangedValue());
  StringName string_name_property;

  GODOT_PROPERTY(EmitChangedValue());
  Transform2D transform2D_property;

  GODOT_PROPERTY(EmitChangedValue());
  Transform3D transform3D_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector2 vector2_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector2i vector2i_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector3 vector3_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector3i vector3i_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector4 vector4_property;

  GODOT_PROPERTY(EmitChangedValue());
  Vector4i vector4i_property;

  GODOT_PROPERTY(EmitChangedValue());
  Node* node_property = nullptr;

  GODOT_PROPERTY(EmitChangedValue());
  Ref<Resource> resource_property;

  GODOT_ENUM();

  enum Enum { ENUM_VALUE };

  GODOT_PROPERTY(EmitChangedValue());
  Enum enum_property = ENUM_VALUE;
};
}  // namespace Tests

GODOT_GENERATED_GLOBAL();
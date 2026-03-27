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

#include "virtual_functions.generated.h"

namespace Tests
{
GODOT_CLASS();

class VirtualFunctions : Node
{
  GODOT_GENERATED_BODY();

  GODOT_FUNCTION(ScriptVirtual);
  AABB aabb_function();

  GODOT_FUNCTION(ScriptVirtual);
  Array array_function();

  GODOT_FUNCTION(ScriptVirtual);
  TypedArray<Resource> typed_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  Basis basis_function();

  GODOT_FUNCTION(ScriptVirtual);
  bool bool_function();

  GODOT_FUNCTION(ScriptVirtual);
  Callable callable_function();

  GODOT_FUNCTION(ScriptVirtual);
  Color color_function();

  GODOT_FUNCTION(ScriptVirtual);
  Dictionary dictionary_function();

  GODOT_FUNCTION(ScriptVirtual);
  TypedDictionary<int, Resource> typed_dictionary_function();

  GODOT_FUNCTION(ScriptVirtual);
  float float_function();

  GODOT_FUNCTION(ScriptVirtual);
  real_t real_t_function();

  GODOT_FUNCTION(ScriptVirtual);
  double double_function();

  GODOT_FUNCTION(ScriptVirtual);
  int int_function();

  GODOT_FUNCTION(ScriptVirtual);
  long long_function();

  GODOT_FUNCTION(ScriptVirtual);
  NodePath node_path_function();

  GODOT_FUNCTION(ScriptVirtual);
  Object* object_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedByteArray packed_byte_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedColorArray packed_color_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedFloat32Array packed_float32_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedFloat64Array packed_float64_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedInt32Array packed_int32_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedInt64Array packed_int64_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedStringArray packed_string_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedVector2Array packed_vector2_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedVector3Array packed_vector3_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  PackedVector4Array packed_vector4_array_function();

  GODOT_FUNCTION(ScriptVirtual);
  Plane plane_function();

  GODOT_FUNCTION(ScriptVirtual);
  Projection projection_function();

  GODOT_FUNCTION(ScriptVirtual);
  Quaternion quaternion_function();

  GODOT_FUNCTION(ScriptVirtual);
  Rect2 rect2_function();

  GODOT_FUNCTION(ScriptVirtual);
  Rect2i rect2i_function();

  GODOT_FUNCTION(ScriptVirtual);
  Signal signal_function();

  GODOT_FUNCTION(ScriptVirtual);
  String string_function();

  GODOT_FUNCTION(ScriptVirtual);
  StringName string_name_function();

  GODOT_FUNCTION(ScriptVirtual);
  Transform2D transform2D_function();

  GODOT_FUNCTION(ScriptVirtual);
  Transform3D transform3D_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector2 vector2_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector2i vector2i_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector3 vector3_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector3i vector3i_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector4 vector4_function();

  GODOT_FUNCTION(ScriptVirtual);
  Vector4i vector4i_function();

  GODOT_FUNCTION(ScriptVirtual);
  Node* node_function();

  GODOT_FUNCTION(ScriptVirtual);
  Ref<Resource> resource_function();
};
} // namespace Tests
GODOT_GENERATED_GLOBAL();
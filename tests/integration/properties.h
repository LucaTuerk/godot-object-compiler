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

  GOC_ASSERT_QUALIFIED_NAME("Tests::Properties", generated_source)
  GOC_ASSERT_PROP_BOUND("variant_property", "NIL");
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
  GOC_ASSERT_PROP_BOUND("packed_float32_array_property",
                        "PACKED_FLOAT32_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_float64_array_property",
                        "PACKED_FLOAT64_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_int32_array_property", "PACKED_INT32_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_int64_array_property", "PACKED_INT64_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_string_array_property", "PACKED_STRING_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector2_array_property",
                        "PACKED_VECTOR2_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector3_array_property",
                        "PACKED_VECTOR3_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector4_array_property",
                        "PACKED_VECTOR4_ARRAY");
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
};

GOC_INTEGRATION_TEST(GeneratePropertyBindsEmitChangedValue) {
  GOC_INTEGRATION_TEST_GEN_FILE(
      "tests/files/integration_tests/properties_emit_changed_value.h");

  GOC_ASSERT_QUALIFIED_NAME("Tests::PropertiesEmitChangedValue",
                            generated_source)
  GOC_ASSERT_PROP_BOUND("aabb_property", "AABB");
  GOC_ASSERT_SIGNAL_BOUND("aabb_property_changed", "AABB");
  GOC_ASSERT_PROP_BOUND("array_property", "ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("array_property_changed", "ARRAY");
  GOC_ASSERT_PROP_BOUND("typed_array_property", "ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("typed_array_property_changed", "ARRAY");
  GOC_ASSERT_PROP_BOUND("basis_property", "BASIS");
  GOC_ASSERT_SIGNAL_BOUND("basis_property_changed", "BASIS");
  GOC_ASSERT_PROP_BOUND("bool_property", "BOOL");
  GOC_ASSERT_SIGNAL_BOUND("bool_property_changed", "BOOL");
  GOC_ASSERT_PROP_BOUND("callable_property", "CALLABLE");
  GOC_ASSERT_SIGNAL_BOUND("callable_property_changed", "CALLABLE");
  GOC_ASSERT_PROP_BOUND("color_property", "COLOR");
  GOC_ASSERT_SIGNAL_BOUND("color_property_changed", "COLOR");
  GOC_ASSERT_PROP_BOUND("typed_dictionary_property", "DICTIONARY");
  GOC_ASSERT_SIGNAL_BOUND("typed_dictionary_property_changed", "DICTIONARY");
  GOC_ASSERT_PROP_BOUND("float_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND("float_property_changed", "FLOAT");
  GOC_ASSERT_PROP_BOUND("real_t_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND("real_t_property_changed", "FLOAT");
  GOC_ASSERT_PROP_BOUND("double_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND("double_property_changed", "FLOAT");
  GOC_ASSERT_PROP_BOUND("int_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND("int_property_changed", "INT");
  GOC_ASSERT_PROP_BOUND("long_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND("long_property_changed", "INT");
  GOC_ASSERT_PROP_BOUND("node_path_property", "NODE_PATH");
  GOC_ASSERT_SIGNAL_BOUND("node_path_property_changed", "NODE_PATH");
  GOC_ASSERT_PROP_BOUND("object_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND("object_property_changed", "OBJECT");
  GOC_ASSERT_PROP_BOUND("packed_byte_array_property", "PACKED_BYTE_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_byte_array_property_changed",
                          "PACKED_BYTE_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_color_array_property", "PACKED_COLOR_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_color_array_property_changed",
                          "PACKED_COLOR_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_float32_array_property",
                        "PACKED_FLOAT32_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_float32_array_property_changed",
                          "PACKED_FLOAT32_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_float64_array_property",
                        "PACKED_FLOAT64_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_float64_array_property_changed",
                          "PACKED_FLOAT64_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_int32_array_property", "PACKED_INT32_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_int32_array_property_changed",
                          "PACKED_INT32_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_int64_array_property", "PACKED_INT64_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_int64_array_property_changed",
                          "PACKED_INT64_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_string_array_property", "PACKED_STRING_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_string_array_property_changed",
                          "PACKED_STRING_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector2_array_property",
                        "PACKED_VECTOR2_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_vector2_array_property_changed",
                          "PACKED_VECTOR2_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector3_array_property",
                        "PACKED_VECTOR3_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_vector3_array_property_changed",
                          "PACKED_VECTOR3_ARRAY");
  GOC_ASSERT_PROP_BOUND("packed_vector4_array_property",
                        "PACKED_VECTOR4_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND("packed_vector4_array_property_changed",
                          "PACKED_VECTOR4_ARRAY");
  GOC_ASSERT_PROP_BOUND("plane_property", "PLANE");
  GOC_ASSERT_SIGNAL_BOUND("plane_property_changed", "PLANE");
  GOC_ASSERT_PROP_BOUND("projection_property", "PROJECTION");
  GOC_ASSERT_SIGNAL_BOUND("projection_property_changed", "PROJECTION");
  GOC_ASSERT_PROP_BOUND("quaternion_property", "QUATERNION");
  GOC_ASSERT_SIGNAL_BOUND("quaternion_property_changed", "QUATERNION");
  GOC_ASSERT_PROP_BOUND("rect2_property", "RECT2");
  GOC_ASSERT_SIGNAL_BOUND("rect2_property_changed", "RECT2");
  GOC_ASSERT_PROP_BOUND("rect2i_property", "RECT2I");
  GOC_ASSERT_SIGNAL_BOUND("rect2i_property_changed", "RECT2I");
  GOC_ASSERT_PROP_BOUND("signal_property", "SIGNAL");
  GOC_ASSERT_SIGNAL_BOUND("signal_property_changed", "SIGNAL");
  GOC_ASSERT_PROP_BOUND("string_property", "STRING");
  GOC_ASSERT_SIGNAL_BOUND("string_property_changed", "STRING");
  GOC_ASSERT_PROP_BOUND("string_name_property", "STRING_NAME");
  GOC_ASSERT_SIGNAL_BOUND("string_name_property_changed", "STRING_NAME");
  GOC_ASSERT_PROP_BOUND("transform2D_property", "TRANSFORM2D");
  GOC_ASSERT_SIGNAL_BOUND("transform2D_property_changed", "TRANSFORM2D");
  GOC_ASSERT_PROP_BOUND("transform3D_property", "TRANSFORM3D");
  GOC_ASSERT_SIGNAL_BOUND("transform3D_property_changed", "TRANSFORM3D");
  GOC_ASSERT_PROP_BOUND("vector2_property", "VECTOR2");
  GOC_ASSERT_SIGNAL_BOUND("vector2_property_changed", "VECTOR2");
  GOC_ASSERT_PROP_BOUND("vector2i_property", "VECTOR2I");
  GOC_ASSERT_SIGNAL_BOUND("vector2i_property_changed", "VECTOR2I");
  GOC_ASSERT_PROP_BOUND("vector3_property", "VECTOR3");
  GOC_ASSERT_SIGNAL_BOUND("vector3_property_changed", "VECTOR3");
  GOC_ASSERT_PROP_BOUND("vector3i_property", "VECTOR3I");
  GOC_ASSERT_SIGNAL_BOUND("vector3i_property_changed", "VECTOR3I");
  GOC_ASSERT_PROP_BOUND("vector4_property", "VECTOR4");
  GOC_ASSERT_SIGNAL_BOUND("vector4_property_changed", "VECTOR4");
  GOC_ASSERT_PROP_BOUND("vector4i_property", "VECTOR4I");
  GOC_ASSERT_SIGNAL_BOUND("vector4i_property_changed", "VECTOR4I");
  GOC_ASSERT_PROP_BOUND("node_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND("node_property_changed", "OBJECT");
  GOC_ASSERT_PROP_BOUND("resource_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND("resource_property_changed", "OBJECT");
  GOC_ASSERT_PROP_BOUND("enum_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND("enum_property_changed", "INT");

  return TEST_RESULT_SUCCESS;
};

GOC_INTEGRATION_TEST(GeneratePropertyBindsEmitChanged) {
  GOC_INTEGRATION_TEST_GEN_FILE(
      "tests/files/integration_tests/properties_emit_changed.h");

  GOC_ASSERT_QUALIFIED_NAME("Tests::PropertiesEmitChanged", generated_source)
  GOC_ASSERT_PROP_BOUND("variant_property", "NIL");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("variant_property_changed");
  GOC_ASSERT_PROP_BOUND("aabb_property", "AABB");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("aabb_property_changed")
  GOC_ASSERT_PROP_BOUND("array_property", "ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("array_property_changed")
  GOC_ASSERT_PROP_BOUND("typed_array_property", "ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("typed_array_property_changed")
  GOC_ASSERT_PROP_BOUND("basis_property", "BASIS");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("basis_property_changed")
  GOC_ASSERT_PROP_BOUND("bool_property", "BOOL");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("bool_property_changed")
  GOC_ASSERT_PROP_BOUND("callable_property", "CALLABLE");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("callable_property_changed")
  GOC_ASSERT_PROP_BOUND("color_property", "COLOR");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("color_property_changed")
  GOC_ASSERT_PROP_BOUND("typed_dictionary_property", "DICTIONARY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("typed_dictionary_property_changed")
  GOC_ASSERT_PROP_BOUND("float_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("float_property_changed")
  GOC_ASSERT_PROP_BOUND("real_t_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("real_t_property_changed")
  GOC_ASSERT_PROP_BOUND("double_property", "FLOAT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("double_property_changed")
  GOC_ASSERT_PROP_BOUND("int_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("int_property_changed")
  GOC_ASSERT_PROP_BOUND("long_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("long_property_changed")
  GOC_ASSERT_PROP_BOUND("node_path_property", "NODE_PATH");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("node_path_property_changed")
  GOC_ASSERT_PROP_BOUND("object_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("object_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_byte_array_property", "PACKED_BYTE_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_byte_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_color_array_property", "PACKED_COLOR_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_color_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_float32_array_property",
                        "PACKED_FLOAT32_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_float32_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_float64_array_property",
                        "PACKED_FLOAT64_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_float64_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_int32_array_property", "PACKED_INT32_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_int32_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_int64_array_property", "PACKED_INT64_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_int64_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_string_array_property", "PACKED_STRING_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_string_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_vector2_array_property",
                        "PACKED_VECTOR2_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_vector2_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_vector3_array_property",
                        "PACKED_VECTOR3_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_vector3_array_property_changed")
  GOC_ASSERT_PROP_BOUND("packed_vector4_array_property",
                        "PACKED_VECTOR4_ARRAY");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("packed_vector4_array_property_changed")
  GOC_ASSERT_PROP_BOUND("plane_property", "PLANE");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("plane_property_changed")
  GOC_ASSERT_PROP_BOUND("projection_property", "PROJECTION");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("projection_property_changed")
  GOC_ASSERT_PROP_BOUND("quaternion_property", "QUATERNION");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("quaternion_property_changed")
  GOC_ASSERT_PROP_BOUND("rect2_property", "RECT2");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("rect2_property_changed")
  GOC_ASSERT_PROP_BOUND("rect2i_property", "RECT2I");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("rect2i_property_changed")
  GOC_ASSERT_PROP_BOUND("signal_property", "SIGNAL");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("signal_property_changed")
  GOC_ASSERT_PROP_BOUND("string_property", "STRING");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("string_property_changed")
  GOC_ASSERT_PROP_BOUND("string_name_property", "STRING_NAME");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("string_name_property_changed")
  GOC_ASSERT_PROP_BOUND("transform2D_property", "TRANSFORM2D");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("transform2D_property_changed")
  GOC_ASSERT_PROP_BOUND("transform3D_property", "TRANSFORM3D");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("transform3D_property_changed")
  GOC_ASSERT_PROP_BOUND("vector2_property", "VECTOR2");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector2_property_changed")
  GOC_ASSERT_PROP_BOUND("vector2i_property", "VECTOR2I");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector2i_property_changed")
  GOC_ASSERT_PROP_BOUND("vector3_property", "VECTOR3");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector3_property_changed")
  GOC_ASSERT_PROP_BOUND("vector3i_property", "VECTOR3I");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector3i_property_changed")
  GOC_ASSERT_PROP_BOUND("vector4_property", "VECTOR4");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector4_property_changed")
  GOC_ASSERT_PROP_BOUND("vector4i_property", "VECTOR4I");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("vector4i_property_changed")
  GOC_ASSERT_PROP_BOUND("node_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("node_property_changed")
  GOC_ASSERT_PROP_BOUND("resource_property", "OBJECT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("resource_property_changed")
  GOC_ASSERT_PROP_BOUND("enum_property", "INT");
  GOC_ASSERT_SIGNAL_BOUND_NO_ARGS("enum_property_changed")

  return TEST_RESULT_SUCCESS;
};

GOC_INTEGRATION_TEST(GenerateCustomPropertyBinds) {
  GOC_INTEGRATION_TEST_GEN_FILE(
      "tests/files/integration_tests/custom_properties.h");

  GOC_ASSERT_CUSTOM_PROP_BOUND("aabb_property", "AABB");
  GOC_ASSERT_CUSTOM_PROP_BOUND("array_property", "ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("typed_array_property", "ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("basis_property", "BASIS");
  GOC_ASSERT_CUSTOM_PROP_BOUND("bool_property", "BOOL");
  GOC_ASSERT_CUSTOM_PROP_BOUND("callable_property", "CALLABLE");
  GOC_ASSERT_CUSTOM_PROP_BOUND("color_property", "COLOR");
  GOC_ASSERT_CUSTOM_PROP_BOUND("dictionary_property", "DICTIONARY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("typed_dictionary_property", "DICTIONARY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("float_property", "FLOAT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("real_t_property", "FLOAT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("double_property", "FLOAT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("int_property", "INT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("long_property", "INT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("node_path_property", "NODE_PATH");
  GOC_ASSERT_CUSTOM_PROP_BOUND("object_property", "OBJECT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_byte_array_property",
                               "PACKED_BYTE_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_color_array_property",
                               "PACKED_COLOR_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_float32_array_property",
                               "PACKED_FLOAT32_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_float64_array_property",
                               "PACKED_FLOAT64_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_int32_array_property",
                               "PACKED_INT32_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_int64_array_property",
                               "PACKED_INT64_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_string_array_property",
                               "PACKED_STRING_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_vector2_array_property",
                               "PACKED_VECTOR2_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_vector3_array_property",
                               "PACKED_VECTOR3_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("packed_vector4_array_property",
                               "PACKED_VECTOR4_ARRAY");
  GOC_ASSERT_CUSTOM_PROP_BOUND("plane_property", "PLANE");
  GOC_ASSERT_CUSTOM_PROP_BOUND("projection_property", "PROJECTION");
  GOC_ASSERT_CUSTOM_PROP_BOUND("quaternion_property", "QUATERNION");
  GOC_ASSERT_CUSTOM_PROP_BOUND("rect2_property", "RECT2");
  GOC_ASSERT_CUSTOM_PROP_BOUND("rect2i_property", "RECT2I");
  GOC_ASSERT_CUSTOM_PROP_BOUND("signal_property", "SIGNAL");
  GOC_ASSERT_CUSTOM_PROP_BOUND("string_property", "STRING");
  GOC_ASSERT_CUSTOM_PROP_BOUND("string_name_property", "STRING_NAME");
  GOC_ASSERT_CUSTOM_PROP_BOUND("transform2D_property", "TRANSFORM2D");
  GOC_ASSERT_CUSTOM_PROP_BOUND("transform3D_property", "TRANSFORM3D");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector2_property", "VECTOR2");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector2i_property", "VECTOR2I");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector3_property", "VECTOR3");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector3i_property", "VECTOR3I");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector4_property", "VECTOR4");
  GOC_ASSERT_CUSTOM_PROP_BOUND("vector4i_property", "VECTOR4I");
  GOC_ASSERT_CUSTOM_PROP_BOUND("node_property", "OBJECT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("resource_property", "OBJECT");
  GOC_ASSERT_CUSTOM_PROP_BOUND("enum_property", "INT");

  return TEST_RESULT_SUCCESS;
};
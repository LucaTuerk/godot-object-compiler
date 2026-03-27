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
#include "common.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/namespace.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(GenerateFunctionBinds)
{
  GOC_INTEGRATION_TEST_GEN_FILE("tests/files/integration_tests/functions.h");

  GOC_ASSERT_QUALIFIED_NAME("Tests::Functions", generated_source);
  GOC_ASSERT_FUNC_BOUND("variant_function");
  GOC_ASSERT_FUNC_BOUND("aabb_function");
  GOC_ASSERT_FUNC_BOUND("array_function");
  GOC_ASSERT_FUNC_BOUND("typed_array_function");
  GOC_ASSERT_FUNC_BOUND("basis_function");
  GOC_ASSERT_FUNC_BOUND("bool_function");
  GOC_ASSERT_FUNC_BOUND("callable_function");
  GOC_ASSERT_FUNC_BOUND("color_function");
  GOC_ASSERT_FUNC_BOUND("dictionary_function");
  GOC_ASSERT_FUNC_BOUND("typed_dictionary_function");
  GOC_ASSERT_FUNC_BOUND("float_function");
  GOC_ASSERT_FUNC_BOUND("real_t_function");
  GOC_ASSERT_FUNC_BOUND("double_function");
  GOC_ASSERT_FUNC_BOUND("int_function");
  GOC_ASSERT_FUNC_BOUND("long_function");
  GOC_ASSERT_FUNC_BOUND("node_path_function");
  GOC_ASSERT_FUNC_BOUND("object_function");
  GOC_ASSERT_FUNC_BOUND("packed_byte_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_color_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_float32_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_float64_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_int32_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_int64_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_string_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_vector2_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_vector3_array_function");
  GOC_ASSERT_FUNC_BOUND("packed_vector4_array_function");
  GOC_ASSERT_FUNC_BOUND("plane_function");
  GOC_ASSERT_FUNC_BOUND("projection_function");
  GOC_ASSERT_FUNC_BOUND("quaternion_function");
  GOC_ASSERT_FUNC_BOUND("rect2_function");
  GOC_ASSERT_FUNC_BOUND("rect2i_function");
  GOC_ASSERT_FUNC_BOUND("signal_function");
  GOC_ASSERT_FUNC_BOUND("string_function");
  GOC_ASSERT_FUNC_BOUND("string_name_function");
  GOC_ASSERT_FUNC_BOUND("transform2D_function");
  GOC_ASSERT_FUNC_BOUND("transform3D_function");
  GOC_ASSERT_FUNC_BOUND("vector2_function");
  GOC_ASSERT_FUNC_BOUND("vector2i_function");
  GOC_ASSERT_FUNC_BOUND("vector3_function");
  GOC_ASSERT_FUNC_BOUND("vector3i_function");
  GOC_ASSERT_FUNC_BOUND("vector4_function");
  GOC_ASSERT_FUNC_BOUND("vector4i_function");
  GOC_ASSERT_FUNC_BOUND("node_function");
  GOC_ASSERT_FUNC_BOUND("resource_function");

  return TEST_RESULT_SUCCESS;
};
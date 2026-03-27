/**************************************************************************/
/* signals.h                                                              */
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

GOC_INTEGRATION_TEST(GenerateSignalBinds)
{
    GOC_INTEGRATION_TEST_GEN_FILE("tests/files/integration_tests/signals.h");

    GOC_ASSERT_QUALIFIED_NAME("Tests::Signals", generated_source);
    GOC_ASSERT_SIGNAL_BOUND("variant_signal", "NIL");
    GOC_ASSERT_SIGNAL_BOUND("aabb_signal", "AABB");
    GOC_ASSERT_SIGNAL_BOUND("array_signal", "ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("typed_array_signal", "ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("basis_signal", "BASIS");
    GOC_ASSERT_SIGNAL_BOUND("bool_signal", "BOOL");
    GOC_ASSERT_SIGNAL_BOUND("callable_signal", "CALLABLE");
    GOC_ASSERT_SIGNAL_BOUND("color_signal", "COLOR");
    GOC_ASSERT_SIGNAL_BOUND("dictionary_signal", "DICTIONARY");
    GOC_ASSERT_SIGNAL_BOUND("typed_dictionary_signal", "DICTIONARY");
    GOC_ASSERT_SIGNAL_BOUND("float_signal", "FLOAT");
    GOC_ASSERT_SIGNAL_BOUND("real_t_signal", "FLOAT");
    GOC_ASSERT_SIGNAL_BOUND("double_signal", "FLOAT");
    GOC_ASSERT_SIGNAL_BOUND("int_signal", "INT");
    GOC_ASSERT_SIGNAL_BOUND("long_signal", "INT");
    GOC_ASSERT_SIGNAL_BOUND("node_path_signal", "NODE_PATH");
    GOC_ASSERT_SIGNAL_BOUND("object_signal", "OBJECT");
    GOC_ASSERT_SIGNAL_BOUND("packed_byte_array_signal", "PACKED_BYTE_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_color_array_signal", "PACKED_COLOR_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_float32_array_signal", "PACKED_FLOAT32_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_float64_array_signal", "PACKED_FLOAT64_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_int32_array_signal", "PACKED_INT32_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_int64_array_signal", "PACKED_INT64_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_string_array_signal", "PACKED_STRING_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_vector2_array_signal", "PACKED_VECTOR2_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_vector3_array_signal", "PACKED_VECTOR3_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("packed_vector4_array_signal", "PACKED_VECTOR4_ARRAY");
    GOC_ASSERT_SIGNAL_BOUND("plane_signal", "PLANE");
    GOC_ASSERT_SIGNAL_BOUND("projection_signal", "PROJECTION");
    GOC_ASSERT_SIGNAL_BOUND("quaternion_signal", "QUATERNION");
    GOC_ASSERT_SIGNAL_BOUND("rect2_signal", "RECT2");
    GOC_ASSERT_SIGNAL_BOUND("rect2i_signal", "RECT2I");
    GOC_ASSERT_SIGNAL_BOUND("signal_signal", "SIGNAL");
    GOC_ASSERT_SIGNAL_BOUND("string_signal", "STRING");
    GOC_ASSERT_SIGNAL_BOUND("string_name_signal", "STRING_NAME");
    GOC_ASSERT_SIGNAL_BOUND("transform2D_signal", "TRANSFORM2D");
    GOC_ASSERT_SIGNAL_BOUND("transform3D_signal", "TRANSFORM3D");
    GOC_ASSERT_SIGNAL_BOUND("vector2_signal", "VECTOR2");
    GOC_ASSERT_SIGNAL_BOUND("vector2i_signal", "VECTOR2I");
    GOC_ASSERT_SIGNAL_BOUND("vector3_signal", "VECTOR3");
    GOC_ASSERT_SIGNAL_BOUND("vector3i_signal", "VECTOR3I");
    GOC_ASSERT_SIGNAL_BOUND("vector4_signal", "VECTOR4");
    GOC_ASSERT_SIGNAL_BOUND("vector4i_signal", "VECTOR4I");
    GOC_ASSERT_SIGNAL_BOUND("node_signal", "OBJECT");
    GOC_ASSERT_SIGNAL_BOUND("resource_signal", "OBJECT");

    return TEST_RESULT_SUCCESS;
};
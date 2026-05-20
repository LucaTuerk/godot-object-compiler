/**************************************************************************/
/* signals_const_ref.h                                                    */
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

#include "signals_const_ref.generated.h"

namespace Tests
{
    GODOT_CLASS();

    class Signals : Node
    {
        GODOT_GENERATED_BODY();

        GODOT_SIGNAL();
        void variant_signal(const Variant& p_param);

        GODOT_SIGNAL();
        void aabb_signal(const AABB& p_param);

        GODOT_SIGNAL();
        void array_signal(const Array& p_param);

        GODOT_SIGNAL();
        void typed_array_signal(const TypedArray<Resource>& p_param);

        GODOT_SIGNAL();
        void basis_signal(const Basis& p_param);

        GODOT_SIGNAL();
        void bool_signal(const bool& p_param);

        GODOT_SIGNAL();
        void callable_signal(const Callable& p_param);

        GODOT_SIGNAL();
        void color_signal(const Color& p_param);

        GODOT_SIGNAL();
        void dictionary_signal(const Dictionary& p_param);

        GODOT_SIGNAL();
        void typed_dictionary_signal(const TypedDictionary<int, Resource>& p_param);

        GODOT_SIGNAL();
        void float_signal(const float& p_param);

        GODOT_SIGNAL();
        void real_t_signal(const real_t& p_param);

        GODOT_SIGNAL();
        void double_signal(const double& p_param);

        GODOT_SIGNAL();
        void int_signal(const int& p_param);

        GODOT_SIGNAL();
        void long_signal(const long& p_param);

        GODOT_SIGNAL();
        void node_path_signal(const NodePath& p_param);

        GODOT_SIGNAL();
        void object_signal(const Object*& p_param);

        GODOT_SIGNAL();
        void packed_byte_array_signal(const PackedByteArray& p_param);

        GODOT_SIGNAL();
        void packed_color_array_signal(const PackedColorArray& p_param);

        GODOT_SIGNAL();
        void packed_float32_array_signal(const PackedFloat32Array& p_param);

        GODOT_SIGNAL();
        void packed_float64_array_signal(const PackedFloat64Array& p_param);

        GODOT_SIGNAL();
        void packed_int32_array_signal(const PackedInt32Array& p_param);

        GODOT_SIGNAL();
        void packed_int64_array_signal(const PackedInt64Array& p_param);

        GODOT_SIGNAL();
        void packed_string_array_signal(const PackedStringArray& p_param);

        GODOT_SIGNAL();
        void packed_vector2_array_signal(const PackedVector2Array& p_param);

        GODOT_SIGNAL();
        void packed_vector3_array_signal(const PackedVector3Array& p_param);

        GODOT_SIGNAL();
        void packed_vector4_array_signal(const PackedVector4Array& p_param);

        GODOT_SIGNAL();
        void plane_signal(const Plane& p_param);

        GODOT_SIGNAL();
        void projection_signal(const Projection& p_param);

        GODOT_SIGNAL();
        void quaternion_signal(const Quaternion& p_param);

        GODOT_SIGNAL();
        void rect2_signal(const Rect2& p_param);

        GODOT_SIGNAL();
        void rect2i_signal(const Rect2i& p_param);

        GODOT_SIGNAL();
        void signal_signal(const Signal& p_param);

        GODOT_SIGNAL();
        void string_signal(const String& p_param);

        GODOT_SIGNAL();
        void string_name_signal(const StringName& p_param);

        GODOT_SIGNAL();
        void transform2D_signal(const Transform2D& p_param);

        GODOT_SIGNAL();
        void transform3D_signal(const Transform3D& p_param);

        GODOT_SIGNAL();
        void vector2_signal(const Vector2& p_param);

        GODOT_SIGNAL();
        void vector2i_signal(const Vector2i& p_param);

        GODOT_SIGNAL();
        void vector3_signal(const Vector3& p_param);

        GODOT_SIGNAL();
        void vector3i_signal(const Vector3i& p_param);

        GODOT_SIGNAL();
        void vector4_signal(const Vector4& p_param);

        GODOT_SIGNAL();
        void vector4i_signal(const Vector4i& p_param);

        GODOT_SIGNAL();
        void node_signal(const Node*& p_param);

        GODOT_SIGNAL();
        void resource_signal(const Ref<Resource>& p_param);
    };
} // namespace Tests

GODOT_GENERATED_GLOBAL();
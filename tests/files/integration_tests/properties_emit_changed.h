/**************************************************************************/
/* properties_emit_changed.h                                              */
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
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/variant/aabb.hpp"
#include "godot_cpp/variant/array_helpers.hpp"
#include "godot_cpp/variant/basis.hpp"
#include "godot_cpp/variant/callable_custom.hpp"
#include "godot_cpp/variant/char_string.hpp"
#include "godot_cpp/variant/color.hpp"
#include "godot_cpp/variant/plane.hpp"
#include "godot_cpp/variant/projection.hpp"
#include "godot_cpp/variant/quaternion.hpp"
#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/rect2i.hpp"
#include "godot_cpp/variant/transform2d.hpp"
#include "godot_cpp/variant/transform3d.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/typed_dictionary.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/variant_internal.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include "godot_cpp/variant/vector3i.hpp"
#include "godot_cpp/variant/vector4.hpp"
#include "godot_cpp/variant/vector4i.hpp"
#include "properties_emit_changed.generated.h"

using namespace godot;
namespace Tests
{

    GODOT_CLASS();
    class PropertiesEmitChanged : public Node
    {
        GODOT_GENERATED_BODY();

        GODOT_PROPERTY(EmitChanged());
        Variant variant_property;

        GODOT_PROPERTY(EmitChanged());
        AABB aabb_property;

        GODOT_PROPERTY(EmitChanged());
        Array array_property;

        GODOT_PROPERTY(EmitChanged());
        TypedArray<Resource> typed_array_property;

        GODOT_PROPERTY(EmitChanged());
        Basis basis_property;

        GODOT_PROPERTY(EmitChanged());
        bool bool_property = false;

        GODOT_PROPERTY(EmitChanged());
        Callable callable_property;

        GODOT_PROPERTY(EmitChanged());
        Color color_property;

        GODOT_PROPERTY(EmitChanged());
        Dictionary dictionary_property;

        GODOT_PROPERTY(EmitChanged());
        TypedDictionary<int, Resource> typed_dictionary_property;

        GODOT_PROPERTY(EmitChanged());
        float float_property = 0;

        GODOT_PROPERTY(EmitChanged());
        real_t real_t_property = 0;

        GODOT_PROPERTY(EmitChanged());
        double double_property = 0;

        GODOT_PROPERTY(EmitChanged());
        int int_property = 0;

        GODOT_PROPERTY(EmitChanged());
        long long_property = 0;

        GODOT_PROPERTY(EmitChanged());
        NodePath node_path_property;

        GODOT_PROPERTY(EmitChanged());
        Object* object_property = nullptr;

        GODOT_PROPERTY(EmitChanged());
        PackedByteArray packed_byte_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedColorArray packed_color_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedFloat32Array packed_float32_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedFloat64Array packed_float64_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedInt32Array packed_int32_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedInt64Array packed_int64_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedStringArray packed_string_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedVector2Array packed_vector2_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedVector3Array packed_vector3_array_property;

        GODOT_PROPERTY(EmitChanged());
        PackedVector4Array packed_vector4_array_property;

        GODOT_PROPERTY(EmitChanged());
        Plane plane_property;

        GODOT_PROPERTY(EmitChanged());
        Projection projection_property;

        GODOT_PROPERTY(EmitChanged());
        Quaternion quaternion_property;

        GODOT_PROPERTY(EmitChanged());
        Rect2 rect2_property;

        GODOT_PROPERTY(EmitChanged());
        Rect2i rect2i_property;

        GODOT_PROPERTY(EmitChanged());
        Signal signal_property;

        GODOT_PROPERTY(EmitChanged());
        String string_property;

        GODOT_PROPERTY(EmitChanged());
        StringName string_name_property;

        GODOT_PROPERTY(EmitChanged());
        Transform2D transform2D_property;

        GODOT_PROPERTY(EmitChanged());
        Transform3D transform3D_property;

        GODOT_PROPERTY(EmitChanged());
        Vector2 vector2_property;

        GODOT_PROPERTY(EmitChanged());
        Vector2i vector2i_property;

        GODOT_PROPERTY(EmitChanged());
        Vector3 vector3_property;

        GODOT_PROPERTY(EmitChanged());
        Vector3i vector3i_property;

        GODOT_PROPERTY(EmitChanged());
        Vector4 vector4_property;

        GODOT_PROPERTY(EmitChanged());
        Vector4i vector4i_property;

        GODOT_PROPERTY(EmitChanged());
        Node* node_property = nullptr;

        GODOT_PROPERTY(EmitChanged());
        Ref<Resource> resource_property;

        GODOT_ENUM();
        enum Enum { ENUM_VALUE };

        GODOT_PROPERTY(EmitChanged());
        Enum enum_property = ENUM_VALUE;
    };
} // namespace Tests

GODOT_GENERATED_GLOBAL();

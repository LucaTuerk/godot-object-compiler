/**************************************************************************/
/* assumptions.h                                                          */
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
#include "generated_assumptions/all.h"
#include "library/core/assumption.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  namespace AssumedGodotTypes {

    inline Assumption<AssumeType<Class>> Object{
        {"Object"}, "Assume that a class named \"Object\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> RefCounted{
        {"RefCounted"}, "Assume that a class named \"RefCounted\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Resource{
        {"Resource"}, "Assume that a class named \"Resource\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Node{{"Node"}, "Assume that a class named \"Node\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> GodotRef{
        {"Ref", 1}, "Assume that a class named \"Ref\" with 1 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Array{{"Array"}, "Assume that a class named \"Array\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> TypedArray{
        {"TypedArray", 1}, "Assume that a class named \"TypedArray\" with 1 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Dictionary{
        {"Dictionary"}, "Assume that a class named \"Dictionary\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> TypedDictionary{{"TypedDictionary", 2},
        "Assume that a class named \"TypedDictionary\" with 2 template parameter exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> ModuleInitializationLevel{{"ModuleInitializationLevel"},
        "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> PropertyHintEnum{
        {"PropertyHint"}, "Assume that an enum type of name \"PropertyHint\" exists in the TypeDB."};

    inline Assumption<AssumeType<Enum>> PropertyUsageFlagsEnum{
        {"PropertyUsageFlags"}, "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

    inline Assumption<AssumeType<Class>> Variant{
        {"Variant"}, "Assume that a class named \"Variant\" exists in the TypeDB"};

    inline Assumption<AssumeType<Enum>> VariantTypeEnum{
        {"Variant::Type"}, "Assume that an enum type of name \"Variant::Type\" exists in the TypeDB"};

    inline Assumption<AssumeType<Define>> GDCLASS{
        {"GDCLASS"}, "Assume that a macro of name \"GDCLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_CLASS{
        {"GDREGISTER_CLASS"}, "Assume that a macro of name \"GDREGISTER_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_VIRTUAL_CLASS{{"GDREGISTER_VIRTUAL_CLASS"},
        "Assume that a macro of name \"GDREGISTER_VIRTUAL_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_ABSTRACT_CLASS{{"GDREGISTER_ABSTRACT_CLASS"},
        "Assume that a macro of name \"GDREGISTER_ABSTRACT_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_INTERNAL_CLASS{{"GDREGISTER_INTERNAL_CLASS"},
        "Assume that a macro of name \"GDREGISTER_INTERNAL_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDREGISTER_RUNTIME_CLASS{{"GDREGISTER_RUNTIME_CLASS"},
        "Assume that a macro of name \"GDREGISTER_RUNTIME_CLASS\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDVIRTUAL_CALL{
        {"GDVIRTUAL_CALL"}, "Assume that a macro of name \"GDVIRTUAL_CALL\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> GDVIRTUAL_BIND{
        {"GDVIRTUAL_BIND"}, "Assume that a macro of name \"GDVIRTUAL_BIND\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> BIND_ENUM_CONSTANT{
        {"BIND_ENUM_CONSTANT"}, "Assume that a macro of name \"BIND_ENUM_CONSTANT\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> BIND_BITFIELD_FLAG{
        {"BIND_BITFIELD_FLAG"}, "Assume that a macro of name \"BIND_BITFIELD_FLAG\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> VARIANT_ENUM_CAST{
        {"VARIANT_ENUM_CAST"}, "Assume that a macro of name \"VARIANT_ENUM_CAST\" exisits in the TypeDB."};

    inline Assumption<AssumeType<Define>> VARIANT_BITFIELD_CAST{
        {"VARIANT_BITFIELD_CAST"}, "Assume that a macro of name \"VARIANT_BITFIELD_CAST\" exisits in the TypeDB."};

    bool validate_assumptions();

  }

}
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
#include "library/tree/syntax/enum.h"
#include "library/type_db.h"

namespace GodotObjectCompiler
{

    namespace AssumedGodotTypes
    {

        inline Assumption ClassDB{
            AssumeType<Class>{"ClassDB"},
            "Assume that a class named \"ClassDB\" exists in the TypeDB."};

        inline Assumption Object{
            AssumeType<Class>{"Object"},
            "Assume that a class named \"Object\" exists in the TypeDB."};

        inline Assumption RefCounted{
            AssumeType<Class>{"RefCounted"},
            "Assume that a class named \"RefCounted\" exists in the TypeDB."};

        inline Assumption Resource{
            AssumeType<Class>{"Resource"},
            "Assume that a class named \"Resource\" exists in the TypeDB."};

        inline Assumption Node{
            AssumeType<Class>{"Node"}, "Assume that a class named \"Node\" exists in the TypeDB."};

        inline Assumption GodotRef{
            AssumeType<Class>{"Ref", 1},
            "Assume that a class named \"Ref\" with 1 template parameter exists in the TypeDB."};

        inline Assumption Array{
            AssumeType<Class>{"Array"},
            "Assume that a class named \"Array\" exists in the TypeDB."};

        inline Assumption TypedArray{
            AssumeType<Class>{"TypedArray", 1}, "Assume that a class named \"TypedArray\" with 1 "
                                                "template parameter exists in the TypeDB."};

        inline Assumption Dictionary{
            AssumeType<Class>{"Dictionary"},
            "Assume that a class named \"Dictionary\" exists in the TypeDB."};

        inline Assumption TypedDictionary{
            AssumeType<Class>{"TypedDictionary", 2},
            "Assume that a class named \"TypedDictionary\" with 2 template parameter exists in the "
            "TypeDB."};

        inline Assumption ModuleInitializationLevel{
            AssumeType<Enum>{"ModuleInitializationLevel"},
            "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

        inline Assumption PropertyHintEnum{
            AssumeType<Enum>{"PropertyHint"},
            "Assume that an enum type of name \"PropertyHint\" exists in the TypeDB."};

        inline Assumption PropertyUsageFlagsEnum{
            AssumeType<Enum>{"PropertyUsageFlags"},
            "Assume that an enum type of name \"ModuleInitializationLevel\" exists in the TypeDB."};

        inline Assumption ADD_PROPERTY{
            AssumeType<Define>{"ADD_PROPERTY"},
            "Assume that a define of name \"ADD_PROPERTY\" exists in the TypeDB."};

        inline Assumption ADD_GROUP{
            AssumeType<Define>{"ADD_GROUP"},
            "Assume that a define of name \"ADD_GROUP\" exists in the TypeDB."};

        inline Assumption ADD_SUBGROUP{
            AssumeType<Define>{"ADD_SUBGROUP"},
            "Assume that a define of name \"ADD_SUBGROUP\" exists in the TypeDB."};

        inline Assumption Variant{
            AssumeType<Class>{"Variant"},
            "Assume that a class named \"Variant\" exists in the TypeDB"};

        inline Assumption VariantTypeEnum{
            AssumeType<Enum>{"Variant::Type"},
            "Assume that an enum type of name \"Variant::Type\" exists in the TypeDB"};

        inline Assumption VariantTypeInt{
            AssumeType<EnumValue>{"Variant::Type::INT"},
            "Assume thet an enum value of name \"Variant::Type::INT\" exists in the TypeDB"};

        inline Assumption VariantTypeFloat{
            AssumeType<EnumValue>{"Variant::Type::FLOAT"},
            "Assume thet an enum value of name \"Variant::Type::FLOAT\" exists in the TypeDB"};

        inline Assumption VariantTypeBool{
            AssumeType<EnumValue>{"Variant::Type::BOOL"},
            "Assume thet an enum value of name \"Variant::Type::BOOL\" exists in the TypeDB"};

        inline Assumption GDCLASS{
            AssumeType<Define>{"GDCLASS"},
            "Assume that a macro of name \"GDCLASS\" exisits in the TypeDB."};

        inline Assumption GDREGISTER_CLASS{
            AssumeType<Define>{"GDREGISTER_CLASS"},
            "Assume that a macro of name \"GDREGISTER_CLASS\" exisits in the TypeDB."};

        inline Assumption GDREGISTER_VIRTUAL_CLASS{
            AssumeType<Define>{"GDREGISTER_VIRTUAL_CLASS"},
            "Assume that a macro of name \"GDREGISTER_VIRTUAL_CLASS\" exisits in the TypeDB."};

        inline Assumption GDREGISTER_ABSTRACT_CLASS{
            AssumeType<Define>{"GDREGISTER_ABSTRACT_CLASS"},
            "Assume that a macro of name \"GDREGISTER_ABSTRACT_CLASS\" exisits in the TypeDB."};

        inline Assumption GDREGISTER_INTERNAL_CLASS{
            AssumeType<Define>{"GDREGISTER_INTERNAL_CLASS"},
            "Assume that a macro of name \"GDREGISTER_INTERNAL_CLASS\" exisits in the TypeDB."};

        inline Assumption GDREGISTER_RUNTIME_CLASS{
            AssumeType<Define>{"GDREGISTER_RUNTIME_CLASS"},
            "Assume that a macro of name \"GDREGISTER_RUNTIME_CLASS\" exisits in the TypeDB."};

        inline Assumption GDVIRTUAL_CALL{
            AssumeType<Define>{"GDVIRTUAL_CALL"},
            "Assume that a macro of name \"GDVIRTUAL_CALL\" exisits in the TypeDB."};

        inline Assumption GDVIRTUAL_BIND{
            AssumeType<Define>{"GDVIRTUAL_BIND"},
            "Assume that a macro of name \"GDVIRTUAL_BIND\" exisits in the TypeDB."};

        inline Assumption BIND_ENUM_CONSTANT{
            AssumeType<Define>{"BIND_ENUM_CONSTANT"},
            "Assume that a macro of name \"BIND_ENUM_CONSTANT\" exisits in the TypeDB."};

        inline Assumption BIND_BITFIELD_FLAG{
            AssumeType<Define>{"BIND_BITFIELD_FLAG"},
            "Assume that a macro of name \"BIND_BITFIELD_FLAG\" exisits in the TypeDB."};

        inline Assumption VARIANT_ENUM_CAST{
            AssumeType<Define>{"VARIANT_ENUM_CAST"},
            "Assume that a macro of name \"VARIANT_ENUM_CAST\" exisits in the TypeDB."};

        inline Assumption VARIANT_BITFIELD_CAST{
            AssumeType<Define>{"VARIANT_BITFIELD_CAST"},
            "Assume that a macro of name \"VARIANT_BITFIELD_CAST\" exisits in the TypeDB."};

        inline Assumption MultiplayerPeerTransferMode{
            AssumeType<Enum>{"MultiplayerPeer::TransferMode"},
            "Assume that an enum of name \"MultiplayerPeer::TransferMode\" exists in the TypeDB"};

        inline Assumption MultiplayerAPI{
            AssumeType<Class>{"MultiplayerAPI"},
            "Assume that an class of name \"MultiplayerAPI\" exists in the TypeDB"};

        inline Assumption MultiplayerPeer{
            AssumeType<Class>{"MultiplayerPeer"},
            "Assume that an class of name \"MultiplayerPeer\" exists in the TypeDB"};

        inline Assumption MultiplayerAPIRPCMode{
            AssumeType<Enum>{"MultiplayerAPI::RPCMode"},
            "Assume that an enum of name \"MultiplayerAPI::RPCMode\" exists in the TypeDB"};

        inline Assumption StringName{
            AssumeType<Class>{"StringName"},
            "Assume that a class of name \"StringName\" exists in the TypeDB"};

        inline Assumption Callable{
            AssumeType<Class>{"Callable"},
            "Assume that a class of name \"Callable\" exists in the TypeDB"};

        inline Assumption List{
            AssumeType<Class>{"List", 2},
            "Assume that a class of name \"List\" exists in the TypeDB"};

        inline Assumption PropertyInfo{
            AssumeType<Class>{"PropertyInfo"},
            "Assume that a class of name \"PropertyInfo\" exists in the TypeDB"};

        bool validate_assumptions();

    } // namespace AssumedGodotTypes

} // namespace GodotObjectCompiler

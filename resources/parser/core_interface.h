/**************************************************************************/
/* core_interface.h                                                       */
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

namespace godot
{
    enum ModuleInitializationLevel {
        MODULE_INITIALIZATION_LEVEL_CORE,
        MODULE_INITIALIZATION_LEVEL_SERVERS,
        MODULE_INITIALIZATION_LEVEL_SCENE,
        MODULE_INITIALIZATION_LEVEL_EDITOR,
        MODULE_INITIALIZATION_LEVEL_MAX
    };

    template <typename T> class Ref
    {
    };

    class Array
    {
    };

    template <typename T> class TypedArray
    {
    };

    template <typename T> class List
    {
    };

    class Dictionary
    {
    };

    template <typename K, typename V> class TypedDictionary
    {
    };

    class Variant
    {
    };

    class StringName
    {
    };

    class Callable
    {
    };

    class PropertyInfo
    {
    };

} // namespace godot

#define ADD_PROPERTY
#define ADD_GROUP
#define ADD_SUBGROUP
#define GDCLASS
#define GDREGISTER_CLASS
#define GDREGISTER_VIRTUAL_CLASS
#define GDREGISTER_ABSTRACT_CLASS
#define GDREGISTER_INTERNAL_CLASS
#define GDREGISTER_RUNTIME_CLASS
#define GDVIRTUAL_CALL
#define GDVIRTUAL_BIND
#define BIND_ENUM_CONSTANT
#define BIND_BITFIELD_FLAG
#define VARIANT_ENUM_CAST
#define VARIANT_BITFIELD_CAST
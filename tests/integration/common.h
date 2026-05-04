/**************************************************************************/
/* common.h                                                               */
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
#include "library/core/core.h"

bool generate_files(
    const GodotObjectCompiler::String& p_path, GodotObjectCompiler::String& r_generated_header,
    GodotObjectCompiler::String& r_generated_source, GodotObjectCompiler::String& r_register_header,
    GodotObjectCompiler::String& r_register_source);

GodotObjectCompiler::String get_line_that_contains(
    const GodotObjectCompiler::String& p_content,
    const GodotObjectCompiler::Vector<GodotObjectCompiler::String>& p_search);

GodotObjectCompiler::Size find_line_that_contains(
    const GodotObjectCompiler::String& p_content,
    const GodotObjectCompiler::Vector<GodotObjectCompiler::String>& p_search,
    GodotObjectCompiler::Size p_start_line = 0);

bool enum_bound(
    const char* p_enum_name, bool p_is_flags, std::initializer_list<const char*>&& p_check_values,
    const GodotObjectCompiler::String& p_generated_source,
    const GodotObjectCompiler::String& p_generated_header);

bool property_bound(
    const char* p_property_name, const char* p_variant_type,
    const GodotObjectCompiler::String& p_generated_header,
    const GodotObjectCompiler::String& p_generated_source);

bool custom_property_bound(
    const char* p_property_name, const char* p_variant_type,
    const GodotObjectCompiler::String& p_generated_source);

bool signal_bound(
    const char* p_signal_name, const char* p_variant_type,
    const GodotObjectCompiler::String& p_generated_header,
    const GodotObjectCompiler::String& p_generated_source, bool p_no_args);

bool function_bound(
    const char* p_function_name, const GodotObjectCompiler::String& p_generated_header,
    const GodotObjectCompiler::String& p_generated_source);

bool virtual_function_bound(
    const char* p_function_name, const char* p_type,
    const GodotObjectCompiler::String& p_generated_header,
    const GodotObjectCompiler::String& p_generated_source);

bool class_included(
    const char* p_class_name, const GodotObjectCompiler::String& p_generated_source);

#define GOC_INTEGRATION_TEST_GEN_FILE(file)                                                        \
    String generated_header, generated_source, register_header, register_source;                   \
    bool success = generate_files(                                                                 \
        file, generated_header, generated_source, register_header, register_source);               \
    GOC_TEST_ASSERT(success, "Failed to generate files from \"%s\"", file)

#define GOC_INTEGRATION_TEST_GEN_INVALID_FILE(file)                                                \
    String generated_header, generated_source, register_header, register_source;                   \
    bool success = generate_files(                                                                 \
        file, generated_header, generated_source, register_header, register_source);               \
    GOC_TEST_ASSERT(!success, "Invalid file \"%s\" was succesfully generated.", file)

#define GOC_ASSERT_PROP_BOUND(prop, variant_type)                                                  \
    GOC_TEST_ASSERT(                                                                               \
        property_bound(prop, variant_type, generated_header, generated_source),                    \
        format("Property \"%s\" not bound.", prop));

#define GOC_ASSERT_QUALIFIED_NAME(name, generated_source)                                          \
    GOC_TEST_ASSERT(                                                                               \
        string_contains(generated_source, name),                                                   \
        "Qualified name \"%s\" does not appear in the source", name);

#define GOC_ASSERT_ENUM_BOUND(name, ...)                                                           \
    GOC_TEST_ASSERT(                                                                               \
        enum_bound(name, false, {__VA_ARGS__}, generated_source, generated_header),                \
        "Enum \"%s\" not bound.", name);

#define GOC_ASSERT_FLAGS_BOUND(name, ...)                                                          \
    GOC_TEST_ASSERT(                                                                               \
        enum_bound(name, true, {__VA_ARGS__}, generated_source, generated_header),                 \
        "Flags \"%s\" not bound.", name);

#define GOC_ASSERT_CUSTOM_PROP_BOUND(prop, variant_type)                                           \
    GOC_TEST_ASSERT(                                                                               \
        custom_property_bound(prop, variant_type, generated_source),                               \
        format("Property \"%s\" not bound.", prop));

#define GOC_ASSERT_SIGNAL_BOUND(signal, variant_type)                                              \
    GOC_TEST_ASSERT(                                                                               \
        signal_bound(signal, variant_type, generated_header, generated_source, false),             \
        format("Signal \"%s\" not bound.", signal));

#define GOC_ASSERT_SIGNAL_BOUND_NO_ARGS(signal)                                                    \
    GOC_TEST_ASSERT(                                                                               \
        signal_bound(signal, "", generated_header, generated_source, true),                        \
        format("Signal \"%s\" not bound.", signal));

#define GOC_ASSERT_FUNC_BOUND(func)                                                                \
    GOC_TEST_ASSERT(                                                                               \
        function_bound(func, generated_header, generated_source),                                  \
        format("Function \"%s\" not bound.", func));

#define GOC_ASSERT_VIRTUAL_BOUND(func, type)                                                       \
    GOC_TEST_ASSERT(                                                                               \
        virtual_function_bound(func, type, generated_header, generated_source),                    \
        format("Function \"%s\" not bound.", func));

#define GOC_ASSERT_CLASS_INCLUDED(class_name)                                                      \
    GOC_TEST_ASSERT(                                                                               \
        class_included(class_name, generated_source),                                              \
        format("Class \"%s\" not included.", class_name));

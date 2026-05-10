/**************************************************************************/
/* macro_include.h                                                        */
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
#include "library/tree/output/output_transformator.h"
#include "library_godot/generators/godot_macro_include_generator.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(MacroInclude)
{
    GodotMacroIncludeGenerator generator;
    const Ref<Context> context = node_new<Context>();
    generator.generate(nullptr, context);

    OutputTransformator transformator;
    StreamWriter writer;
    transformator.transform(context)->get_output(&writer);

    const String include_content = writer.get_string();

    // Check attributes
    GOC_ASSERT_LINE_CONTAINS(include_content, "Class attribute not found", "define", "GODOT_CLASS");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Enum attribute not found", "define", "GODOT_ENUM");
    GOC_ASSERT_LINE_CONTAINS(
        include_content, "Property attribute not found", "define", "GODOT_PROPERTY");
    GOC_ASSERT_LINE_CONTAINS(
        include_content, "Generated Body attribute not found", "define", "GODOT_GENERATED_BODY");
    GOC_ASSERT_LINE_CONTAINS(
        include_content, "Generated Global attribute not found", "define",
        "GODOT_GENERATED_GLOBAL");
    GOC_ASSERT_LINE_CONTAINS(
        include_content, "Function attribute not found", "define", "GODOT_FUNCTION");

    // Check some parameters
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "LevelScene");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "LevelEditor");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "UsageNone");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "HintRange");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "Flags");
    GOC_ASSERT_LINE_CONTAINS(include_content, "Not found.", "Channel");

    return TEST_RESULT_SUCCESS;
};
/**************************************************************************/
/* extension_api_parser_tests.h                                           */
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
#include "library/tree/predicates.h"
#include "library_godot/parsers/extension_api_parser.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_INTEGRATION_TEST(ExtensionAPIParser)
{
    ExtensionAPIParser parser;
    parser.setup_include_paths(
        TestRegistry::instance()->get_integration_tests_godot_cpp_include_paths());
    const Ref<Context> global_context = node_new<Namespace>();
    const auto api_path = TestRegistry::instance()->get_extension_api();
    const Ref<ParserError> error = parser.parse_file(api_path, global_context);
    GOC_TEST_EQ(
        error, ParserError::OK, "Failed to parse extension api file \"%s\"", api_path.c_str());

    Vector<String> check_classes = {"Node", "Resource", "RefCounted", "Object", "Signal"};
    Vector<String> check_enums = {"Variant::Type", "PropertyHint", "PropertyUsageFlags"};

    for (const String& check_class : check_classes) {
        Ref<Class> _class = global_context->find_descendant<Class>(
            BFS, NamedContextPredicates::name<Class>(check_class.c_str()));
        GOC_TEST_NEQ(
            _class, nullptr, "Failed to find class of name \"%s\" in parsed extension api.");
    }

    for (const String& check_enum : check_enums) {
        Ref<Enum> _enum = global_context->find_descendant<Enum>(
            BFS, NamedContextPredicates::name<Enum>(check_enum.c_str()));
        GOC_TEST_NEQ(_enum, nullptr, "Failed to find enum of name \"%s\" in parsed extension api.");
        GOC_TEST_ASSERT(!_enum->value_names().empty(), "Parsed enum ")
    }
    return TEST_RESULT_SUCCESS;
};
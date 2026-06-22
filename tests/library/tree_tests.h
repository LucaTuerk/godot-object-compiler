/**************************************************************************/
/* tree_tests.h                                                           */
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
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_TEST(AddRemoveChild)
{
    Vector<Ref<Node>> children;
    Ref<Context> context = node_new<Context>();

    for (int i = 0; i < 1000; ++i) {
        auto node = node_new<Node>();
        children.push_back(node);
        context->add_child(node);
    }

    GOC_TEST_EQ(children.size(), context->get_child_count(), "Invalid child count after insert");

    for (const auto& node : children) {
        GOC_TEST_EQ(node->get_parent(), context, "Node parent is invalid.");
    }

    for (const auto& node : children) {
        context->remove_child(node);
    }

    GOC_TEST_EQ(context->get_child_count(), 0, "Invalid child count after remove");

    for (const auto& node : children) {
        GOC_TEST_EQ(node->get_parent(), nullptr, "Node parent is invalid.");
    }
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(AddChildBefore)
{
    Vector<Ref<Node>> children;
    const Ref<Context> context = node_new<Context>();

    for (int i = 0; i < 10; ++i) {
        auto node = node_new<Node>();
        children.push_back(node);
        context->add_child(node);
    }

    const auto node = node_new<Node>();

    for (int i = 0; i < 10; ++i) {
        context->add_child_before(node, children[i]);
        GOC_TEST_EQ(node->get_next_sibling(), children[i], "Node next sibling is invalid.");
        context->remove_child(node);
    }

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ReplaceChild)
{
    Vector<Ref<Context>> children;
    const Ref<Context> context = node_new<Context>();

    for (int i = 0; i < 10; ++i) {
        auto node = node_new<Context>();
        children.push_back(node);
        context->add_child(node);
    }

    Ref<Node> child_node = children[5]->B<Context>();
    Ref<Context> new_context = node_new<Context>();

    context->replace_child(children[5], new_context, true);
    GOC_TEST_EQ(context->get_child(5), new_context, "Failed to replace child");
    GOC_TEST_EQ(child_node->get_parent(), new_context, "Failed to take child");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NamespaceLazy)
{
    Ref<Class> class_c;
    Ref<Enum> enum_2;
    Ref<Namespace> _namespace = B<Namespace>()[{
        B<Identifier>("Namespace"),
        B<Body>()[{
            B<Enum>()[{
                B<Identifier>("Enum"),
                B<EnumValues>()[{B<EnumValue>()[{B<Identifier>("ENUM_VALUE")}]}]}],
            B<Class>()[B<Identifier>("A"), B<BaseClasses>(), B<Body>()],
            B<Class>()[{
                B<Identifier>("B"), B<BaseClasses>(),
                B<Body>()[{
                    R<Class>(&class_c)
                        [B<Identifier>("C"), B<BaseClasses>(),
                         B<Body>()[{R<Enum>(&enum_2)[{
                             B<Identifier>("Enum2"),
                             B<EnumValues>()[{B<EnumValue>()[{B<Identifier>("ENUM_VALUE2")}]}]}]}]],
                }]}]}]}];

    GOC_TEST_EQ(_namespace->classes().size(), 2, "Invalid class count.");
    GOC_TEST_EQ(_namespace->classes_recursive().size(), 3, "Invalid recursive class count.");
    GOC_TEST_EQ(class_c->namespaces().size(), 2, "Invalid namespace count.");
    GOC_TEST_EQ(_namespace->enums().size(), 1, "Invalid enum count.");
    GOC_TEST_EQ(_namespace->enums_recursive().size(), 2, "Invalid recursive enum count.");

    return TEST_RESULT_SUCCESS;
};

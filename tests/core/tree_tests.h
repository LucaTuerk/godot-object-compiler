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

GOC_TEST(AddChildrenTest)
{
    Ref<Context> context = node_new<Context>();

    for (Size i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            context->add_child(node_new<Node>());
        } else {
            context->create_child<Node>();
        }
    }

    GOC_TEST_EQ(context->get_child_count(), 1000, "Invalid child count.");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(RemoveChildrenTest)
{
    Ref<Context> context = node_new<Context>();

    for (Size i = 0; i < 1000; ++i) {
        context->create_child<Node>();
    }

    for (Size i = 0; i < 500; ++i) {
        Ref<Node> child = context->get_child<Node>(rand() % (1000 - i));
        GOC_TEST_ASSERT(child, "Could not get child.");
        context->remove_child(child);
    }

    GOC_TEST_EQ(context->get_child_count(), 500, "Invalid child count.");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(ReparentTest)
{
    Ref<Context> a = node_new<Context>();
    Ref<Context> b = node_new<Context>();

    for (Size i = 0; i < 1000; ++i) {
        a->create_child<Node>();
    }

    for (Size i = 0; i < 500; ++i) {
        Ref<Node> child = a->get_child<Node>(rand() % (1000 - i));
        GOC_TEST_ASSERT(child, "Could not get child.");
        b->add_child(child);
    }

    GOC_TEST_EQ(a->get_child_count(), 500, "Invalid child count.");
    GOC_TEST_EQ(b->get_child_count(), 500, "Invalid child count.");
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NeighborsTest)
{
    Ref<Context> context = node_new<Context>();

    for (Size i = 0; i < 1000; ++i) {
        context->create_child<Node>();
    }

    GOC_TEST_EQ(context->get_child_count(), 1000, "Invalid child count.");

    Size forward_size = 0;
    Ref<Node> current = context->get_child(0);
    while (current) {
        forward_size++;
        current = current->get_next_sibling();
    }

    Size reverse_size = 0;
    current = context->get_child(-1);
    while (current) {
        reverse_size++;
        current = current->get_previous_sibling();
    }

    GOC_TEST_EQ(forward_size, 1000, "Invalid forward iteration neighbor number");
    GOC_TEST_EQ(reverse_size, 1000, "Invalid reverse iteration neighbor number");
    return TEST_RESULT_SUCCESS;
};

/**************************************************************************/
/* node_db_tests.h                                                        */
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

#define GOC_TEST_ASSERT_CREATE_TYPE(node_type)                                                     \
    {                                                                                              \
        Ref<Node> created = LibraryContext::instance()->get_node_db()->create(#node_type);         \
        GOC_TEST_ASSERT(created, "Failed to create node of type \"%s\"", #node_type);              \
        GOC_TEST_EQ(                                                                               \
            created->get_type(), String(#node_type), "Failed to create node of type \"%s\"",       \
            #node_type);                                                                           \
    }

using namespace GodotObjectCompiler;

GOC_TEST(CreateTest)
{
    GOC_TEST_ASSERT_CREATE_TYPE(Node)
    GOC_TEST_ASSERT_CREATE_TYPE(AccessSpecifier)
    GOC_TEST_ASSERT_CREATE_TYPE(Context)
    GOC_TEST_ASSERT_CREATE_TYPE(Identifier)
    GOC_TEST_ASSERT_CREATE_TYPE(Literal)
    GOC_TEST_ASSERT_CREATE_TYPE(Inline)
    GOC_TEST_ASSERT_CREATE_TYPE(Override)
    GOC_TEST_ASSERT_CREATE_TYPE(SigndednessModifier)
    GOC_TEST_ASSERT_CREATE_TYPE(Static)
    GOC_TEST_ASSERT_CREATE_TYPE(TypeQualifier)
    GOC_TEST_ASSERT_CREATE_TYPE(Virtual)
    GOC_TEST_ASSERT_CREATE_TYPE(Error)
    GOC_TEST_ASSERT_CREATE_TYPE(PlaceholderType)
    GOC_TEST_ASSERT_CREATE_TYPE(Pointer)
    GOC_TEST_ASSERT_CREATE_TYPE(Reference)
    GOC_TEST_ASSERT_CREATE_TYPE(BaseClasses)
    GOC_TEST_ASSERT_CREATE_TYPE(Body)
    GOC_TEST_ASSERT_CREATE_TYPE(NamedContext)
    GOC_TEST_ASSERT_CREATE_TYPE(Enum)
    GOC_TEST_ASSERT_CREATE_TYPE(EnumValues)
    GOC_TEST_ASSERT_CREATE_TYPE(EnumValue)
    GOC_TEST_ASSERT_CREATE_TYPE(Argument)
    GOC_TEST_ASSERT_CREATE_TYPE(Arguments)
    GOC_TEST_ASSERT_CREATE_TYPE(Parameter)
    GOC_TEST_ASSERT_CREATE_TYPE(Parameters)
    GOC_TEST_ASSERT_CREATE_TYPE(TemplateArguments)
    GOC_TEST_ASSERT_CREATE_TYPE(TemplateParameters)
    GOC_TEST_ASSERT_CREATE_TYPE(Include)
    GOC_TEST_ASSERT_CREATE_TYPE(Operator)
    GOC_TEST_ASSERT_CREATE_TYPE(Define)
    GOC_TEST_ASSERT_CREATE_TYPE(Function)
    GOC_TEST_ASSERT_CREATE_TYPE(Field)
    GOC_TEST_ASSERT_CREATE_TYPE(Member)
    GOC_TEST_ASSERT_CREATE_TYPE(Namespace)
    GOC_TEST_ASSERT_CREATE_TYPE(Type)
    GOC_TEST_ASSERT_CREATE_TYPE(Class)
    GOC_TEST_ASSERT_CREATE_TYPE(Struct)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotClassAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotEnumAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotFunctionAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotPropertyAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotPropertyCategoryAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotPropertyGroupAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotPropertySubgroupAttribute)
    GOC_TEST_ASSERT_CREATE_TYPE(GodotSignalAttribute)

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NodeIDTest)
{
    Ref<Context> context = node_new<Context>();

    for (Size i = 0; i < 1000; ++i) {
        if (LibraryContext::instance()->get_node_db()->get<Node>(i) != nullptr) {
            print_ln("Unlikely id collison in node id test. Ignore!");
            return TEST_RESULT_IGNORED;
        }

        Ref<Node> child = context->create_child<Node>();
        UID new_uid =
            LibraryContext::instance()->get_node_db()->request_id_change(child->get_id(), i);
        GOC_TEST_EQ(new_uid, i, "Could not change id.");
    }

    for (Size uid = 0; uid < 1000; ++uid) {
        Ref<Node> node = LibraryContext::instance()->get_node_db()->get<Node>(uid);
        GOC_TEST_EQ(node, context->get_child(uid), "Invalid node found for id %d", uid);
    }

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NodeIDRemovedTest)
{
    Vector<UID> uids;

    for (Size i = 0; i < 1000; ++i) {
        Ref<Node> node = node_new<Node>();
        uids.push_back(node->get_id());
    }

    for (auto& uid : uids) {
        Ref<Node> node = LibraryContext::instance()->get_node_db()->get<Node>(uid);
        GOC_TEST_ASSERT(!node, "Deleted node is still accessible in the node db");
    }

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NodeIDChangeValid)
{
    Vector<UID> uids;

    for (Size i = 0; i < 1000; ++i) {
        Ref<Node> node = node_new<Node>();
        uids.push_back(node->get_id());
    }

    auto node_db = LibraryContext::instance()->get_node_db();

    for (UID uid : uids) {
        auto before_node = node_db->get<Node>(uid);
        UID new_id = node_db->request_id_change(uid);
        auto after_node = node_db->get<Node>(new_id);
        GOC_TEST_EQ(before_node, after_node, "Node ID change failed");
    }
    return TEST_RESULT_SUCCESS;
};

GOC_TEST(NodeIDChangeInvalid)
{
    Vector<UID> uids;

    for (Size i = 0; i < 20; ++i) {
        Ref<Node> node = node_new<Node>();
        uids.push_back(node->get_id());
    }

    auto node_db = LibraryContext::instance()->get_node_db();

    for (UID uid : uids) {
        auto before_node = node_db->get<Node>(uid);
        try {
            std::ignore = node_db->request_id_change(NodeDB::generate_unique_id(), uid);
        } catch (std::exception& e) {
        }
        auto after_node = node_db->get<Node>(uid);
        GOC_TEST_EQ(before_node, after_node, "Invalid Node ID change modified id");
    }

    for (UID uid : uids) {
        auto before_node = node_db->get<Node>(uid);
        try {
            std::ignore = node_db->request_id_change(INVALID_ID, uid);
        } catch (std::exception& e) {
        }
        auto after_node = node_db->get<Node>(uid);
        GOC_TEST_EQ(before_node, after_node, "Invalid Node ID change modified id");
    }

    return TEST_RESULT_SUCCESS;
};
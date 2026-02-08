/**************************************************************************/
/* class_tests.h                                                          */
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
#include "library/parser/parser.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/type.h"
#include "tests/test_registry.h"

GOC_TEST(ClassSimple) {
  using namespace GodotObjectCompiler;

  TreeSitterParser parser;
  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse_file("tests/files/class_tests/simple_class.h", global_namespace);

  GOC_TEST_EQ(error, ParserError::OK, "Parser error occured.");
  GOC_TEST_ASSERT(global_namespace, "Global namespace is invalid.");

  Vector<Ref<Class>> classes = global_namespace->classes_recursive();

  GOC_TEST_EQ(classes.size(), 1, "Invalid class count.")
  GOC_TEST_EQ(classes[0]->name(), "SimpleClass", "Invalid class name.")

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(ClassSimpleInheritance) {
  using namespace GodotObjectCompiler;

  TreeSitterParser parser;
  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse_file("tests/files/class_tests/simple_inheritance.h", global_namespace);

  GOC_TEST_EQ(error, ParserError::OK, "Parser error occured.");
  GOC_TEST_ASSERT(global_namespace, "Global namespace is invalid.");

  Vector<Ref<Class>> classes = global_namespace->classes_recursive();
  GOC_TEST_EQ(classes.size(), 3, "Invalid class count.")

  Ref<Class> A = global_namespace->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("A"));
  GOC_TEST_ASSERT(A, "Class named A not found");

  Ref<Class> B = global_namespace->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("B"));
  GOC_TEST_ASSERT(B, "Class named B not found");

  Ref<Class> C = global_namespace->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("C"));
  GOC_TEST_ASSERT(C, "Class named C not found");

  Vector<String> A_bases = A->direct_bases_names();
  GOC_TEST_EQ(A_bases.size(), 0, "Invalid base class count.");

  Vector<String> B_bases = B->direct_bases_names();
  GOC_TEST_EQ(B_bases.size(), 1, "Invalid base class count.");
  GOC_TEST_EQ(B_bases[0], "A", "Invalid base class name.");

  Vector<String> C_bases = C->direct_bases_names();
  GOC_TEST_EQ(C_bases.size(), 1, "Invalid base class count.");
  GOC_TEST_EQ(C_bases[0], "B", "Invalid base class name.");

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(ClassMemberFunctions) {
  using namespace GodotObjectCompiler;

  TreeSitterParser parser;
  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse_file("tests/files/class_tests/member_functions.h", global_namespace);

  GOC_TEST_EQ(error, ParserError::OK, "Parser error occured.");
  GOC_TEST_ASSERT(global_namespace, "Global namespace is invalid.");

  Vector<Ref<Class>> classes = global_namespace->classes_recursive();
  GOC_TEST_EQ(classes.size(), 1, "Invalid class count.")

  Ref<Class> A = global_namespace->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("A"));
  GOC_TEST_ASSERT(A, "Class named A not found");

  Vector<String> public_members = {"a", "b", "c", "d", "e", "f"};
  Vector<String> protected_members = {"g", "h", "i", "j", "k", "l"};
  Vector<String> private_members = {"m", "n", "o", "p", "q", "r"};
  Vector<String> parameters = {"", "", "", "int", "bool", "A*"};
  Vector<String> return_types = {"int", "bool", "A*", "int", "bool", "A*"};

  Vector<Pair<Vector<Ref<Function>>, Vector<String>>> functions_names_pairs = {
      {A->public_member_functions(), public_members}, {A->protected_member_functions(), protected_members},
      {A->private_member_functions(), private_members}};

  for (const auto& [functions, names] : functions_names_pairs) {
    Size i = 0;
    for (const Ref<Function>& func : functions) {
      GOC_TEST_ASSERT(func->type(), "Failed to get return type");
      GOC_TEST_EQ(func->name(), names[i], "Wrong function name.")

      GOC_TEST_EQ(func->type()->name(), return_types[i], "Wrong return type.")
      if (!parameters[i].empty()) {
        GOC_TEST_ASSERT(func->parameters(), "Failed to get function arguments.")
        GOC_TEST_EQ(func->parameters()->get_child_count(), 1, "Wrong parameter count.")

        Ref<Parameter> parameter = func->parameters()->get_child(0)->as<Parameter>();
        GOC_TEST_ASSERT(parameter, "Failed to get parameter.")

        Ref<Type> parameter_type = parameter->type();
        GOC_TEST_ASSERT(parameter_type, "Failed to get parameter type.")

        GOC_TEST_EQ(parameter_type->name(), parameters[i], "Wrong parameter type.")
      }
      i++;
    }
  }

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(ClassMemberFields) {
  using namespace GodotObjectCompiler;

  TreeSitterParser parser;
  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse_file("tests/files/class_tests/member_fields.h", global_namespace);

  GOC_TEST_EQ(error, ParserError::OK, "Parser error occured.");
  GOC_TEST_ASSERT(global_namespace, "Global namespace is invalid.");

  Vector<Ref<Class>> classes = global_namespace->classes_recursive();
  GOC_TEST_EQ(classes.size(), 1, "Invalid class count.")

  Ref<Class> A = global_namespace->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("A"));
  GOC_TEST_ASSERT(A, "Class named A not found");

  Vector<String> public_members = {"a", "b", "c"};
  Vector<String> protected_members = {"d", "e", "f"};
  Vector<String> private_members = {"g", "h", "i"};
  Vector<String> types = {"int", "bool", "A*"};

  Vector<Pair<Vector<Ref<Field>>, Vector<String>>> fields_names_pairs = {{A->public_member_fields(), public_members},
      {A->protected_member_fields(), protected_members}, {A->private_member_fields(), private_members}};

  for (const auto& [fields, names] : fields_names_pairs) {
    Size i = 0;
    for (const Ref<Field>& field : fields) {
      GOC_TEST_ASSERT(field->type(), "Failed to get field type");
      GOC_TEST_EQ(field->name(), names[i], "Wrong field name.")

      GOC_TEST_EQ(field->type()->name(), types[i], "Wrong field type.")
      i++;
    }
  }

  return TEST_RESULT_SUCCESS;
};
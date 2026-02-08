#pragma once
#include "library/parser/parser.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax//namespace.h"
#include "tests/test_registry.h"

GOC_TEST(ParserSimpleNamespace) {
  using namespace GodotObjectCompiler;
  TreeSitterParser parser;

  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse("namespace A {}", global_namespace);
  GOC_TEST_EQ(error, ParserError::OK, "ParserError occurred!");

  Vector<Ref<Namespace>> namespaces = global_namespace->find_children<Namespace>(true);
  GOC_TEST_EQ(namespaces.size(), 1, "Invalid namespace count in parsed result!");

  GOC_TEST_EQ(namespaces[0]->name(), "A", "Invalid name for parsed namespace!");
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParserNestedNamespace) {
  using namespace GodotObjectCompiler;
  TreeSitterParser parser;

  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse("namespace A { namespace B {}}", global_namespace);
  GOC_TEST_EQ(error, ParserError::OK, "ParserError occurred!");

  Vector<Ref<Namespace>> namespaces = global_namespace->find_children<Namespace>(true);
  GOC_TEST_EQ(namespaces.size(), 2, "Invalid namespace count in parsed result!");

  Ref<Namespace> namespacesA =
      global_namespace->find_descendant<Namespace>(BFS, NamedContextPredicates::name<Namespace>("A"));
  GOC_TEST_ASSERT(namespacesA, "Failed to find outer namespace of name A");
  Ref<Namespace> namespacesB =
      global_namespace->find_descendant<Namespace>(BFS, NamedContextPredicates::name<Namespace>("B"));
  GOC_TEST_ASSERT(namespacesB, "Failed to find inner namespace of name B");
  GOC_TEST_EQ(namespacesB->qualified_name(), "A::B", "Invalid qualified name for inner namespace B")

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(ParseNestedNamespaces2) {
  using namespace GodotObjectCompiler;
  using namespace NamedContextPredicates;
  TreeSitterParser parser;
  Ref<Namespace> global_namespace = node_new<Namespace>();
  Ref<ParserError> error = parser.parse_file("tests/files/namespace_tests/10_nested_namespaces.h",global_namespace);
  GOC_TEST_EQ(error, ParserError::OK, "ParserError occurred!");

  Vector<Ref<Namespace>> namespaces = global_namespace->find_children<Namespace>(true);
  GOC_TEST_EQ(namespaces.size(), 10, "Invalid namespace count.")

  Ref<Namespace> A = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("A"));
  Ref<Namespace> B = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("B"));
  Ref<Namespace> C = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("C"));
  Ref<Namespace> D = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("D"));
  Ref<Namespace> E = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("E"));
  Ref<Namespace> F = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("F"));
  Ref<Namespace> G = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("G"));
  Ref<Namespace> H = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("H"));
  Ref<Namespace> I = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("I"));
  Ref<Namespace> J = global_namespace->find_descendant<Namespace>(BFS, name<Namespace>("J"));

  GOC_TEST_ASSERT(A, "Failed to find namespace A.")
  GOC_TEST_ASSERT(B, "Failed to find namespace B.")
  GOC_TEST_ASSERT(C, "Failed to find namespace C.")
  GOC_TEST_ASSERT(D, "Failed to find namespace D.")
  GOC_TEST_ASSERT(E, "Failed to find namespace E.")
  GOC_TEST_ASSERT(F, "Failed to find namespace F.")
  GOC_TEST_ASSERT(G, "Failed to find namespace G.")
  GOC_TEST_ASSERT(H, "Failed to find namespace H.")
  GOC_TEST_ASSERT(I, "Failed to find namespace I.")
  GOC_TEST_ASSERT(J, "Failed to find namespace J.")

  GOC_TEST_EQ(A->qualified_name(), "A", "Invalid qualified name for namespace A");
  GOC_TEST_EQ(B->qualified_name(), "A::B", "Invalid qualified name for namespace B");
  GOC_TEST_EQ(C->qualified_name(), "A::B::C", "Invalid qualified name for namespace C");
  GOC_TEST_EQ(D->qualified_name(), "A::D", "Invalid qualified name for namespace D");
  GOC_TEST_EQ(E->qualified_name(), "A::E", "Invalid qualified name for namespace E");
  GOC_TEST_EQ(F->qualified_name(), "F", "Invalid qualified name for namespace F");
  GOC_TEST_EQ(G->qualified_name(), "F::G", "Invalid qualified name for namespace G");
  GOC_TEST_EQ(H->qualified_name(), "F::G::H", "Invalid qualified name for namespace H");
  GOC_TEST_EQ(I->qualified_name(), "F::G::H::I", "Invalid qualified name for namespace I");
  GOC_TEST_EQ(J->qualified_name(), "F::G::H::J", "Invalid qualified name for namespace J");

  return TEST_RESULT_SUCCESS;
};
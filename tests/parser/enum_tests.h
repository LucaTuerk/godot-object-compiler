#pragma once
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/tree/syntax/enum.h"
#include "tests/test_registry.h"

GOC_TEST(EnumValues) {
  using namespace GodotObjectCompiler;
  GOC_TEST_PARSE_FILE("tests/files/enum_tests/enum_values.h")

  Ref<Enum> enum1 = global_namespace->find_descendant<Enum>(BFS, NamedContextPredicates::name<Enum>("TestEnum1"));
  GOC_TEST_ASSERT(enum1, "Failed to find TestEnum1");
  GOC_TEST_ASSERT(vector_contains(enum1->value_names(), "VALUE_A"), "Failed to find TestEnum1::VALUE_A")
  GOC_TEST_ASSERT(vector_contains(enum1->value_names(), "VALUE_B"), "Failed to find TestEnum1::VALUE_B")
  GOC_TEST_ASSERT(vector_contains(enum1->value_names(), "VALUE_C"), "Failed to find TestEnum1::VALUE_C")

  Ref<Enum> enum2 = global_namespace->find_descendant<Enum>(BFS, NamedContextPredicates::name<Enum>("TestEnum2"));
  GOC_TEST_ASSERT(enum2, "Failed to find TestEnum2");
  GOC_TEST_ASSERT(vector_contains(enum2->value_names(), "VALUE_D"), "Failed to find TestEnum1::VALUE_D")
  GOC_TEST_ASSERT(vector_contains(enum2->value_names(), "VALUE_E"), "Failed to find TestEnum1::VALUE_E")
  GOC_TEST_ASSERT(vector_contains(enum2->value_names(), "VALUE_F"), "Failed to find TestEnum1::VALUE_F")

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(EnumValueAssign) {
  using namespace GodotObjectCompiler;
  GOC_TEST_PARSE_FILE("tests/files/enum_tests/enum_value_assign.h")

  Ref<Enum> test_enum = global_namespace->find_descendant<Enum>(BFS, NamedContextPredicates::name<Enum>("TestEnum"));
  GOC_TEST_ASSERT(test_enum, "Failed to find TestEnum");

  int i = 1;
  for (const Ref<EnumValue>& value : test_enum->find_children<EnumValue>()) {
    Ref<Literal> literal = value->find_child<Literal>();
    GOC_TEST_ASSERT(literal, "Failed to find Literal");
    GOC_TEST_EQ(string_to_int(literal->content), i, "Invalid literal.");
    i++;
  }

  Ref<Enum> test_enum2 = global_namespace->find_descendant<Enum>(BFS, NamedContextPredicates::name<Enum>("TestEnum2"));
  GOC_TEST_ASSERT(test_enum2, "Failed to find TestEnum2");

  i = 1;
  for (const Ref<EnumValue>& value : test_enum2->find_children<EnumValue>()) {
    Ref<Literal> literal = value->find_child<Literal>();
    GOC_TEST_ASSERT(literal, "Failed to find Literal");
    if (value->name() == "VALUE_ALL") {
      GOC_TEST_EQ(string_to_int(literal->content), i - 1, "Invalid literal.");
    } else {
      GOC_TEST_EQ(string_to_int(literal->content), i, "Invalid literal.");
    }
    i++;
  }

  Ref<Enum> test_enum3 = global_namespace->find_descendant<Enum>(BFS, NamedContextPredicates::name<Enum>("TestEnum3"));
  GOC_TEST_ASSERT(test_enum3, "Failed to find TestEnum3");

  i = 0;
  for (const Ref<EnumValue>& value : test_enum3->find_children<EnumValue>()) {
    Ref<Literal> literal = value->find_child<Literal>();
    GOC_TEST_ASSERT(literal, "Failed to find Literal");
    if (value->name() == "VALUE_D3") {
      i = 10;
    }
    GOC_TEST_EQ(string_to_int(literal->content), i, "Invalid literal.");
    i++;
  }

  return TEST_RESULT_SUCCESS;
};
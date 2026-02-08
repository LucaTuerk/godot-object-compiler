#pragma once

#include "library/parser/parser.h"
#include "library/tree/syntax//namespace.h"
#include "library/tree/syntax/parser_error.h"
#include "tests/test_registry.h"

GOC_TEST(ParserRandStringFuzz) {
  using namespace GodotObjectCompiler;

  TreeSitterParser parser;

  for (Size i = 0; i < 100; ++i) {
    Ref<Namespace> global_namespace = node_new<Namespace>();
    Ref<ParserError> error = parser.parse(generate_random_string(1000), global_namespace);
    // GOC_TEST_EQ(global_namespace->get_child_count(), 0, "Unexpected results while parsing random string input.")
  }

  return TEST_RESULT_SUCCESS;
};
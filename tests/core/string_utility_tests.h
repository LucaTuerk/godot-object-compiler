/**************************************************************************/
/* string_utility_tests.h                                                 */
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
#include "library/core/string_utilities.h"
#include "test_registry.h"

GOC_TEST(StringReplace) {
  using namespace GodotObjectCompiler;
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringTrim) {
  using namespace GodotObjectCompiler;

  GOC_TEST_EQ(string_trim("  content   "), "content", "Failed to trim string.")
  GOC_TEST_EQ(string_trim("  c content   "), "c content", "Failed to trim string.")
  GOC_TEST_EQ(string_trim("  content content   "), "content content", "Failed to trim string.")
  GOC_TEST_EQ(string_trim(" \n\t  content  \n\t"), "content", "Failed to trim string.")

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringShinkInnerSpace) {
  using namespace GodotObjectCompiler;
  GOC_TEST_EQ(
      string_shrink_inner_space("a     b     c   d   e   f  g"), "a b c d e f g", "Failed to shrink inner space.")
  GOC_TEST_EQ(string_shrink_inner_space("a   \n  b   \n  c   d   e \t  f \r g"), "a b c d e f g",
      "Failed to shrink inner space.")
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringContains) {
  using namespace GodotObjectCompiler;
  GOC_TEST_ASSERT(string_contains("abc", "b"), "Failed to determine string contains.")
  GOC_TEST_ASSERT(!string_contains("abc", "d"), "Failed to determine string contains.")
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringSuffix) {
  using namespace GodotObjectCompiler;
  GOC_TEST_ASSERT(string_suffix("abcdefg", "efg"), "Failed to determine string suffix.")
  GOC_TEST_ASSERT(!string_suffix("abcdefg", "abc"), "Failed to determine string suffix.")
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringPrefix) {
  using namespace GodotObjectCompiler;
  GOC_TEST_ASSERT(string_prefix("abcdefg", "abc"), "Failed to determine string prefix.")
  GOC_TEST_ASSERT(!string_prefix("abcdefg", "efg"), "Failed to determine string prefix.")
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringVectorCombine) {
  using namespace GodotObjectCompiler;
  GOC_TEST_EQ(string_vector_combine({"a", "b", "c"}, ","), "a,b,c", "Failed to combine string vector.");
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringSplit) {
  using namespace GodotObjectCompiler;
  Vector<String> split = string_split("a,b,c", ",");
  GOC_TEST_EQ(split.size(), 3, "Invalid split count.")
  GOC_TEST_EQ(split[0], "a", "Invalid split.")
  GOC_TEST_EQ(split[1], "b", "Invalid split.")
  GOC_TEST_EQ(split[2], "c", "Invalid split.")

  Vector<String> split_substr = string_split("adelimiterbdelimiterc", "delimiter");
  GOC_TEST_EQ(split_substr.size(), 3, "Invalid split count.")
  GOC_TEST_EQ(split_substr[0], "a", "Invalid split.")
  GOC_TEST_EQ(split_substr[1], "b", "Invalid split.")
  GOC_TEST_EQ(split_substr[2], "c", "Invalid split.")

  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringMacroToPascal) {
  using namespace GodotObjectCompiler;
  GOC_TEST_EQ(macro_case_to_pascal_case("TEST_VALUE"), "TestValue", "Failed to convert to pascal case.")
  GOC_TEST_EQ(macro_case_to_pascal_case("TEST_VALUE_123"), "TestValue123", "Failed to convert to pascal case.")
  GOC_TEST_EQ(macro_case_to_pascal_case("TESTVALUE123"), "Testvalue123", "Failed to convert to pascal case.")
  return TEST_RESULT_SUCCESS;
};

GOC_TEST(StringEnumToExposedName) {
  using namespace GodotObjectCompiler;
  GOC_TEST_EQ(cpp_enum_case_to_exposed_enum_case("TEST_VALUE"), "Test Value", "Failed to convert to exposed enum name.")
  GOC_TEST_EQ(cpp_enum_case_to_exposed_enum_case("TEST_VALUE_123_Test"), "Test Value 123 Test",
      "Failed to convert to exposed enum name.")
  GOC_TEST_EQ(cpp_enum_case_to_exposed_enum_case("TESTVALUE12345"), "Testvalue 12345",
      "Failed to convert to  exposed enum name.")
  GOC_TEST_EQ(cpp_enum_case_to_exposed_enum_case("TESTVALUE123TEST"), "Testvalue 123 Test",
      "Failed to convert to  exposed enum name.")
  return TEST_RESULT_SUCCESS;
};

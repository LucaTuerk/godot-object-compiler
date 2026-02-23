#pragma once

#include "../test_registry.h"
#include "library/type_db.h"

using namespace GodotObjectCompiler;

GOC_TEST(MangleNames) {
  GOC_TEST_EQ(TypeDB::mangle_name("Ref", 1), "Ref_T_ARGS_1_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("Ref<T>", INVALID_SIZE), "Ref_T_ARGS_1_", "Invalid mangled name");

  GOC_TEST_EQ(TypeDB::mangle_name("TypedArray", 1), "TypedArray_T_ARGS_1_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("TypedArray<T>", INVALID_SIZE), "TypedArray_T_ARGS_1_", "Invalid mangled name");

  GOC_TEST_EQ(TypeDB::mangle_name("TypedDictionary", 2), "TypedDictionary_T_ARGS_2_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("TypedDictionary<K,V>", INVALID_SIZE), "TypedDictionary_T_ARGS_2_",
      "Invalid mangled na"
      "me");
  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref", 1), "godot/Ref_T_ARGS_1_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref<T>", INVALID_SIZE), "godot/Ref_T_ARGS_1_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("::godot::Ref<T>", 1), "godot/Ref_T_ARGS_1_", "Invalid mangled name.");

  GOC_TEST_EQ(TypeDB::mangle_name("godot::TypedArray", 1), "godot/TypedArray_T_ARGS_1_", "Invalid mangled name");
  GOC_TEST_EQ(
      TypeDB::mangle_name("godot::TypedArray<T>", INVALID_SIZE), "godot/TypedArray_T_ARGS_1_", "Invalid mangled name");

  GOC_TEST_EQ(
      TypeDB::mangle_name("godot::TypedDictionary", 2), "godot/TypedDictionary_T_ARGS_2_", "Invalid mangled name");
  GOC_TEST_EQ(TypeDB::mangle_name("godot::TypedDictionary<K,V>", INVALID_SIZE), "godot/TypedDictionary_T_ARGS_2_",
      "Invalid mangled name");

  GOC_TEST_EQ(TypeDB::mangle_name("A::B< C < X,Y,Z < O,P >, L< W > >, D>", INVALID_SIZE), "A/B_T_ARGS_2_",
      "Invalid mangled name");

  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref<T>::Type<A,B,C,D,E,F,G>::other", INVALID_SIZE),
      "godot/Ref_T_ARGS_1_/Type_T_ARGS_7_/other", "Invalid mangled name.");
  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref<T>::Type<A,B,C,D,E,F,G>::other", 1),
      "godot/Ref_T_ARGS_1_/Type_T_ARGS_7_/other_T_ARGS_1_", "Invalid mangled name.");

  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref<T", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(TypeDB::mangle_name("godot:RefT>", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(
      TypeDB::mangle_name("godot:Ref<T>Hallo", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(
      TypeDB::mangle_name("daksndasnd__?<>fadsadk", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(TypeDB::mangle_name("?<Hmm>", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(TypeDB::mangle_name("godot::Ref<T>::", 1), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  GOC_TEST_EQ(TypeDB::mangle_name("godot:cofeve", INVALID_SIZE), TypeDB::INVALID_NAME, "Invalid valid mangled name.");
  return TEST_RESULT_SUCCESS;
};

inline String random_string(Size p_length) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(20, 255);

  std::string random_string;
  for (Size i = 0; i < p_length; ++i) {
    random_string += static_cast<char>(distribution(generator));
  }
  return random_string;
}

inline bool valid_mangled_or_invalid(const String& mangled_name) {
  return std::all_of(
      mangled_name.begin(), mangled_name.end(), [](char c) { return isalnum(c) || c == '_' || c == '/'; });
}

GOC_TEST(MangleNamesFuzz) {
  return TEST_RESULT_IGNORED;

  // for (Size i = 0; i < 10000; ++i) {
  //   String input = random_string(15);
  //   GOC_TEST_ASSERT(valid_mangled_or_invalid(TypeDB::mangle_name(input, INVALID_SIZE)), "Invalid result for input %s",
  //       input.c_str());
  // }
  // return TEST_RESULT_SUCCESS;
};
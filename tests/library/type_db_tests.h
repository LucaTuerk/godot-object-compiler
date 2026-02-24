/**************************************************************************/
/* type_db_tests.h                                                        */
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

inline String random_string(size_t p_length) {
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(1, 255);

	StreamWriter writer;
	for (size_t i = 0; i < p_length; ++i) {
		writer.write_generic(distribution(generator));
	}
	return writer.get_string();
}

inline bool valid_mangled_or_invalid(const String &mangled_name) {
	return std::all_of(
			mangled_name.begin(), mangled_name.end(), [](char c) { return isalnum(c) || c == '_' || c == '/'; });
}

GOC_TEST(MangleNamesFuzz) {
	for (Size i = 0; i < 1000; ++i) {
		String input = random_string(15);
		auto mangled_name = TypeDB::mangle_name(input, INVALID_SIZE);
		GOC_TEST_ASSERT(valid_mangled_or_invalid(mangled_name), "Invalid result for input");
	}
	return TEST_RESULT_SUCCESS;
};
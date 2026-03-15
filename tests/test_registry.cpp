/**************************************************************************/
/* test_registry.cpp                                                      */
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

#include "test_registry.h"

#include "library/core/string_utilities.h"

namespace GodotObjectCompiler {

bool TestRegistry::register_test(const String &name, TestFunctor functor) {
	auto [_, success] = tests.emplace(name, functor);
	return success;
}

bool TestRegistry::register_integration_test(const String &name, TestFunctor functor) {
	auto [_, success] = integration_tests.emplace(name, functor);
	return success;
}

String TestRegistry::test_generated_folder() {
	return ".goc_tests/.goc/generated";
}

String TestRegistry::test_root_folder() {
	return "tests/files/integration_tests";
}

Vector<String> TestRegistry::get_test_application_arguments(const ProgramPath &p_program_path) {
	Vector<String> result = p_program_path;
	result.emplace_back(format("-R=%s", test_root_folder().c_str()));
	result.emplace_back("-P=.goc_tests/.goc");
	result.emplace_back("-G=.goc_tests/.goc/generated");
	result.emplace_back("-C=.goc_tests/.goc/cache");
	result.emplace_back(format(
			"-I=%s",
			string_vector_combine(TestRegistry::instance()->get_integration_tests_include_paths(), ",")
					.c_str()));
	return result;
}

Vector<String> TestRegistry::get_integration_tests_include_paths() {
	return include_paths;
}

void TestRegistry::set_integration_tests_include_paths(const Vector<String> &p_paths) {
	include_paths = p_paths;
}

const Dictionary<String, TestFunctor> &TestRegistry::get_integration_tests() {
	return integration_tests;
}

const Dictionary<String, TestFunctor> &TestRegistry::get_tests() {
	return tests;
}

bool TestRegister::operator<<(TestFunctor functor) const {
	return TestRegistry::instance()->register_test(name, std::move(functor));
}

bool IntegrationTestRegister::operator<<(TestFunctor functor) const {
	return TestRegistry::instance()->register_integration_test(name, std::move(functor));
}

} // namespace GodotObjectCompiler
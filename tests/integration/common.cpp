/**************************************************************************/
/* common.cpp                                                             */
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

#include "common.h"

#include "application/application.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

bool generate_files(const String &p_path, String &r_generated_header, String &r_generated_source,
		String &r_register_header, String &r_register_source) {
	const Vector<String> args =
			TestRegistry::instance()->get_test_application_arguments({ "generate", format("-S=%s", p_path.c_str()) });
	ApplicationContext context;

	if (Application::setup_context(args, context) != 0) {
		print_err("Failed to setup context.");
		Application::exit_gracefully(context, 1);
		return false;
	}

	ExecutionContext::instance()->test_force_clear_modified_time(p_path);

	if (Application::run_program(context) != 0) {
		print_err("Failed to run program.");
		Application::exit_gracefully(context, 1);
		return false;
	}

	if (Application::cleanup_context(context) != 0) {
		print_err("Failed to cleanup context.");
		return false;
	}

	const String base = path_base(p_path);
	const String relative = path_relative(p_path, TestRegistry::instance()->test_root_folder());

	const String generated_header_path =
			path_concat(TestRegistry::instance()->test_generated_folder(), string_replace(relative, ".h", ".generated.h"));
	const String generated_source_path =
			path_concat(TestRegistry::instance()->test_generated_folder(), string_replace(relative, ".h", ".generated.cpp"));
	const String register_header_path =
			path_concat(TestRegistry::instance()->test_generated_folder(), "generated_register_types.h");
	const String register_source_path =
			path_concat(TestRegistry::instance()->test_generated_folder(), "generated_register_types.cpp");

	if (!file_exists(generated_header_path)) {
		print_err("Failed to generate header.");
		return false;
	}

	if (!file_exists(generated_source_path)) {
		print_err("Failed to generate source.");
		return false;
	}

	if (!file_exists(register_header_path)) {
		print_err("Failed to generate register types header.");
		return false;
	}

	if (!file_exists(register_source_path)) {
		print_err("Failed to generate register types source.");
		return false;
	}

	r_generated_header = read_file(generated_header_path);
	r_generated_source = read_file(generated_source_path);
	r_register_header = read_file(register_header_path);
	r_register_source = read_file(register_source_path);
	return true;
}

bool property_bound(const char *p_property_name, const char *p_variant_type, const String &p_generated_header,
		const String &p_generated_source) {
	const String setter_name = format("set_%s", p_property_name);
	const String getter_name = format("get_%s", p_property_name);

	if (!string_contains(p_generated_header, setter_name)) {
		fmt_print_err("Setter definition for \"%s\" not found in header", p_property_name);
		return false;
	}

	if (!string_contains(p_generated_header, getter_name)) {
		fmt_print_err("Getter definition for \"%s\" not found in header", p_property_name);
		return false;
	}

	if (!string_contains(p_generated_source, setter_name)) {
		fmt_print_err("Setter definition for \"%s\" not found in source", p_property_name);
		return false;
	}

	if (!string_contains(p_generated_source, getter_name)) {
		fmt_print_err("Getter definition for \"%s\" not found in source", p_property_name);
		return false;
	}

	if (get_line_that_contains(p_generated_source, { "ADD_PROPERTY", p_variant_type, p_property_name }).empty()) {
		fmt_print_err("Property binding not found for %s with variant type %s", p_property_name, p_variant_type);
		return false;
	}
	return true;
}

bool custom_property_bound(const char *p_property_name, const char *p_variant_type, const String &p_generated_source) {
	if (get_line_that_contains(p_generated_source, { "ADD_PROPERTY", p_variant_type, p_property_name }).empty()) {
		fmt_print_err("Property binding not found for %s with variant type %s", p_property_name, p_variant_type);
		return false;
	}
	return true;
}

bool signal_bound(const char *p_signal_name, const char *p_variant_type,
		const GodotObjectCompiler::String &p_generated_header, const GodotObjectCompiler::String &p_generated_source) {
	UNUSED(p_generated_header);
	if (get_line_that_contains(p_generated_source, { "ADD_SIGNAL", p_signal_name, p_variant_type }).empty()) {
		fmt_print_err("Function bind for \"%s\" not found in source", p_signal_name);
		return false;
	}
	return true;
}

bool function_bound(const char *p_function_name, const GodotObjectCompiler::String &p_generated_header,
		const GodotObjectCompiler::String &p_generated_source) {
	UNUSED(p_generated_header);
	if (get_line_that_contains(p_generated_source, { "bind_method", p_function_name }).empty()) {
		fmt_print_err("Function bind for \"%s\" not found in source", p_function_name);
		return false;
	}
	return true;
}

bool virtual_function_bound(const char *p_function_name, const char *p_type,
		const GodotObjectCompiler::String &p_generated_header, const GodotObjectCompiler::String &p_generated_source) {
	String virtual_name = format("_%s", p_function_name);
	if (get_line_that_contains(p_generated_source, { "GDVIRTUAL_BIND", virtual_name }).empty()) {
		fmt_print_err("Virtual bind for \"%s\" not found in source", p_function_name);
		return false;
	}

	if (get_line_that_contains(p_generated_source, { virtual_name, "GDVIRTUAL_CALL" }).empty()) {
		fmt_print_err("Virtual call definition for \"%s\" not found in source", p_function_name);
		return false;
	}

	if (get_line_that_contains(p_generated_header, { "GDVIRTUAL", virtual_name, p_type }).empty()) {
		fmt_print_err("Virtual definition for \"%s\" not found in header", p_function_name);
		return false;
	}

	return true;
}

String get_line_that_contains(const String &p_content, const Vector<String> &p_search) {
	for (const String &line : string_split(p_content, "\n")) {
		bool contains_all = true;
		for (const String &search : p_search) {
			if (!string_contains(line, search)) {
				contains_all = false;
			}
		}
		if (contains_all) {
			return line;
		}
	}
	return "";
}

Size find_line_that_contains(const String &p_content, const Vector<String> &p_search, Size p_start_line) {
	std::stringstream strstsr(p_content);
	Size i = 0;
	for (String line; std::getline(strstsr, line);) {
		if (p_start_line < i) {
			const bool all_contained = std::all_of(
					p_search.begin(), p_search.end(), [line](const String &search) { return string_contains(line, search); });
			if (all_contained) {
				return i;
			}
		}
		++i;
	}

	return INVALID_SIZE;
}

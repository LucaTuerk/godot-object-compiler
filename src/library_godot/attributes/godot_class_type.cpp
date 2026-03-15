/**************************************************************************/
/* godot_class_type.cpp                                                   */
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

#include "godot_class_type.h"

#include "library/core/string_utilities.h"
#include "library/tree/syntax/define.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

String GodotClassTypeParameterType::get_return_type() {
	return "GOC_GodotClassType";
}

Vector<String> GodotClassTypeParameterType::get_value_names() {
	return value_names();
}

Vector<IAttributeParameterType::Argument> GodotClassTypeParameterType::get_arguments() {
	return {};
}

bool GodotClassTypeParameterType::get_macro_for_value_name(
		const String &p_value_name, String &r_macro) {
	_value_names_lazy.poke();

	const auto itr = _value_name_to_macro.find(p_value_name);
	if (itr == _value_name_to_macro.end()) {
		r_macro = "";
		return false;
	}

	r_macro = itr->second;
	return true;
}

Vector<String> GodotClassTypeParameterType::_value_names_lazy_get() {
	Vector<String> godot_macros = {
		AssumedGodotTypes::GDREGISTER_CLASS().type->name(),
		AssumedGodotTypes::GDREGISTER_VIRTUAL_CLASS().type->name(),
		AssumedGodotTypes::GDREGISTER_ABSTRACT_CLASS().type->name(),
		AssumedGodotTypes::GDREGISTER_INTERNAL_CLASS().type->name(),
		AssumedGodotTypes::GDREGISTER_RUNTIME_CLASS().type->name(),
	};

	Vector<String> value_names = { "RegisterCustom" };
	for (const String &macro : godot_macros) {
		auto value_name = macro_case_to_pascal_case(string_replace(macro, "GDREGISTER_", ""));
		value_name = value_name == "Class" ? "GodotClass" : value_name;
		_value_name_to_macro[value_name] = macro;
		value_names.push_back(value_name);
	}
	return value_names;
}

} // namespace GodotObjectCompiler
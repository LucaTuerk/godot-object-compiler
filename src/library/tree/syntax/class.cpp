/**************************************************************************/
/* class.cpp                                                              */
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

#include "class.h"

#include "attribute.h"
#include "identifier.h"
#include "library/type_db.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

Vector<String> Class::_direct_bases_names_lazy_get() const {
	Vector<String> result;

	const Ref<BaseClasses> base_classes = find_child<BaseClasses>();
	if (!base_classes) {
		return {};
	}

	for (const Ref<Type> &type : base_classes->find_children<Type>()) {
		result.push_back(type->name());
	}

	return result;
}

Vector<Ref<Class>> Class::_base_classes_lazy_get() {
	Vector<Ref<Identifier>> identifiers;

	const Ref<BaseClasses> bases = find_child<BaseClasses>();
	if (!bases) {
		return {};
	}

	Vector<Ref<Class>> result;
	for (const Ref<Type> &type : bases->find_children<Type>()) {
		if (Ref<Class> base_class =
						ExecutionContext::instance()->get_type_db()->get_type_data<Class>(
								type, this->as<Class>())) {
			result.push_back(base_class);
		}
	}

	return result;
}

String Class::_first_base_qualifed_name_lazy_get() const {
	Vector<Ref<Class>> bases = base_classes();
	if (bases.empty()) {
		return "";
	}
	return bases[0]->qualified_name();
}

Vector<Ref<Attribute>> Class::_attributes_lazy_get() const {
	Vector<Ref<Attribute>> attributes;
	if (!body()) {
		return attributes;
	}

	for (const Ref<Node> &child : body()->get_children()) {
		if (const Ref<Attribute> child_attribute = child->as<Attribute>()) {
			attributes.push_back(child_attribute->as<Attribute>());
		}
	}

	return attributes;
}

bool Class::_has_generated_class_attribute_lazy_get() const {
	return find_descendant<GodotGeneratedBodyAttribute>() != nullptr;
}

bool Class::has_function_named(const String &name) const {
	for (const Ref<Function> &member_function : member_functions()) {
		if (member_function->name() == name) {
			return true;
		}
	}
	return false;
}

bool Class::copy_to(const Ref<Node> &p_other) const {
	COPY_GUARD(Class, Namespace);
	// COPY_LAZY(base_classes);
	return true;
}

Vector<Ref<Function>> Class::_member_functions_lazy_get() const {
	if (!body()) {
		return {};
	}

	return body()->find_children<Function>();
}

Vector<Ref<Function>> Class::_public_member_functions_lazy_get() const {
	Vector<Ref<Function>> result;
	for (const Ref<Function> &member_function : member_functions()) {
		if (member_function->is_public_member()) {
			result.push_back(member_function);
		}
	}
	return result;
}

Vector<Ref<Function>> Class::_protected_member_functions_lazy_get() const {
	Vector<Ref<Function>> result;
	for (const Ref<Function> &member_function : member_functions()) {
		if (member_function->is_protected_member()) {
			result.push_back(member_function);
		}
	}
	return result;
}

Vector<Ref<Function>> Class::_private_member_functions_lazy_get() const {
	Vector<Ref<Function>> result;
	for (const Ref<Function> &member_function : member_functions()) {
		if (member_function->is_private_member()) {
			result.push_back(member_function);
		}
	}
	return result;
}

Vector<Ref<Field>> Class::_member_fields_lazy_get() const {
	if (!body()) {
		return {};
	}

	return body()->find_children<Field>();
}

Vector<Ref<Field>> Class::_public_member_fields_lazy_get() const {
	Vector<Ref<Field>> result;
	for (const Ref<Field> &member_field : member_fields()) {
		if (member_field->is_public_member()) {
			result.push_back(member_field);
		}
	}
	return result;
}

Vector<Ref<Field>> Class::_protected_member_fields_lazy_get() const {
	Vector<Ref<Field>> result;
	for (const Ref<Field> &member_field : member_fields()) {
		if (member_field->is_protected_member()) {
			result.push_back(member_field);
		}
	}
	return result;
}

Vector<Ref<Field>> Class::_private_member_fields_lazy_get() const {
	Vector<Ref<Field>> result;
	for (const Ref<Field> &member_field : member_fields()) {
		if (member_field->is_private_member()) {
			result.push_back(member_field);
		}
	}
	return result;
}

Size Class::_template_parameter_count_lazy_get() const {
	Ref<TemplateParameters> template_parameters = find_child<TemplateParameters>();
	if (!template_parameters) {
		return 0;
	}
	return template_parameters->get_child_count();
}

} // namespace GodotObjectCompiler

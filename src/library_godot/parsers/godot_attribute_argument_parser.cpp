/**************************************************************************/
/* godot_attribute_argument_parser.cpp                                    */
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

#include "godot_attribute_argument_parser.h"

#include "library/core/string_utilities.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/identifier.h"
#include "library_godot/attributes/string_literal_parameter_type.h"

namespace GodotObjectCompiler {

Ref<ParserError> GodotAttributeArgumentParser::parse_attribute_arguments(
		const String &p_content, Ref<Context> p_target) {
	const Ref<Attribute> attribute = weak_attribute.lock();

	if (!attribute) {
		return node_new<ParserError>(
				ERROR, "Invalid attribute parser for . "
					   "Associated attribute has exited scope.");
	}

	const Vector<String> arguments = split_arguments(p_content);

	if (arguments.empty()) {
		return ParserError::OK;
	}

	const String property_macro =
			ExecutionContext::instance()->get_attribute_db()->get_macro_for_attribute(
					attribute->get_type());

	if (const Vector<Ref<IAttributeParameterType>> types =
					ExecutionContext::instance()->get_attribute_db()->get_parameters_for_macro(
							property_macro);
			arguments.size() > types.size()) {
		return node_new<ParserError>(
				ERROR, format("Invalid argument count, expected at most %d but found %d", types.size(), arguments.size()));
	}

	for (const String &argument : arguments) {
		for (const String &single : split_flags(argument)) {
			if (single.empty()) {
				return node_new<ParserError>(
						ERROR,
						format("Invalid empty sub argument found in argument \"%s\"", argument.c_str()));
			}

			Ref<ParserError> error = parse_attribute_argument(single, p_target);

			if (error != ParserError::OK) {
				return error;
			}
		}
	}

	return ParserError::OK;
}

Ref<ParserError> GodotAttributeArgumentParser::parse_attribute_argument(
		const String &p_content, Ref<Context> p_target) {
	const Ref<Attribute> attribute = weak_attribute.lock();
	if (!attribute) {
		return node_new<ParserError>(
				ERROR,
				"Invalid attribute parser for " + p_content + ". Associated attribute has exited scope.");
	}

	const String property_macro =
			ExecutionContext::instance()->get_attribute_db()->get_macro_for_attribute(
					attribute->get_type());
	Vector<Ref<IAttributeParameterType>> types =
			ExecutionContext::instance()->get_attribute_db()->get_parameters_for_macro(property_macro);
	if (types.empty()) {
		return node_new<ParserError>(
				ERROR,
				format("No valid parameter types found for attribute %s", attribute->get_type().c_str()));
	}

	bool no_match = true;

	for (const Ref<IAttributeParameterType> &type : types) {
		if (Ref<StringLiteralParameterType> str_literal =
						std::dynamic_pointer_cast<StringLiteralParameterType>(type);
				str_literal) {
			if (string_enclosed_by(p_content, "\"")) {
				const Ref<Argument> argument = str_literal->create_argument();
				if (!argument) {
					return node_new<ParserError>(
							ERROR, "Failed to create argument node for type " + type->get_return_type());
				}

				p_target->add_child(argument);
				argument->build_child<Literal>(p_content);
				no_match = false;
				break;
			}
		}

		String outer, inner;
		split_outer_inner(p_content, outer, inner);

		if (Vector<String> value_names = type->get_value_names();
				!vector_contains(value_names, outer)) {
			continue;
		}
		no_match = false;

		Ref<Argument> argument_node = type->create_argument();
		if (!argument_node) {
			return node_new<ParserError>(
					ERROR, "Failed to create argument node for type " + type->get_return_type());
		}

		p_target->add_child(argument_node);
		argument_node->build_child<Identifier>(outer);
		Ref<Arguments> inner_arguments = argument_node->build_child<Arguments>();

		if (inner.empty()) {
			return ParserError::OK;
		}

		Vector<String> arguments = split_arguments(inner);
		auto parameters = type->get_arguments();

		if (arguments.size() > parameters.size()) {
			return node_new<ParserError>(
					ERROR, format("Invalid inner argument count for \"%s\", expected at "
								  "most %d but found %d",
								   p_content.c_str(), parameters.size(), arguments.size()));
		}

		if (parameters.size() > arguments.size()) {
			Size optional_count = 0;
			for (const auto &parameter : parameters) {
				if (parameter.optional) {
					optional_count++;
				}
			}

			Size diff = parameters.size() - arguments.size();
			if (diff > optional_count) {
				return node_new<ParserError>(
						ERROR,
						format(
								"Invalid inner argument count %d for \"%s\", "
								"expected %d parameters with %d being optional.",
								arguments.size(), p_content.c_str(), parameters.size(), optional_count));
			}
		}

		for (Size i = 0; i < arguments.size() && i < parameters.size(); i++) {
			if (Ref<ParserError> error =
							parse_inner_arguments(arguments[i], inner_arguments, parameters[i]);
					error != ParserError::OK) {
				error->set_handled();
				return node_new<ParserError>(
						ERROR, format("Failed to parse argument \"%s\". %s", p_content.c_str(), attribute->get_type().c_str(), error->message.c_str()));
			}
		}
	}

	if (no_match) {
		return node_new<ParserError>(
				ERROR, format("Failed to find matching argument type with value "
							  "name \"%s\" for attribute %s",
							   p_content.c_str(), attribute->get_type().c_str()));
	}
	return ParserError::OK;
}

Ref<ParserError> GodotAttributeArgumentParser::parse_inner_arguments(
		const String &p_content, const Ref<Context> &p_target,
		const IAttributeParameterType::Argument &p_parameter) {
	switch (p_parameter.type) {
		case IAttributeParameterType::ARG_STRING:
			p_target->build_child<Argument>().with_child<Literal>(p_content);
			break;
		case IAttributeParameterType::ARG_INTEGER:
			p_target->build_child<Argument>().with_child<Literal>(p_content);
			break;
		default:
			PANIC("Unimplemented IAttributeParameterType %d", static_cast<int>(p_parameter.type));
	}

	return ParserError::OK;
}

} // namespace GodotObjectCompiler

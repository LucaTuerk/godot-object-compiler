
#include "godot_attribute_argument_parser.h"

namespace GodotObjectCompiler {

  Ref<ParserError> GodotAttributeArgumentParser::parse_attribute_arguments(const String& content, Ref<Context> target) {
    const Vector<String> arguments = split_arguments(content);

    if (arguments.empty()) {
      return ParserError::OK;
    }

    for (const String& argument : arguments) {
      for (const String& single : split_flags(argument)) {
        Ref<ParserError> error = parse_attribute_argument(single, target);

        if (error != ParserError::OK) {
          return error;
        }
      }
    }

    return ParserError::OK;
  }

  Ref<ParserError> GodotAttributeArgumentParser::parse_attribute_argument(const String& argument, Ref<Context> target) {
    Ref<Attribute> attribute = weak_attribute.lock();
    if (!attribute) {
      return node_new<ParserError>(
          ERROR, "Invalid attribute parser for " + argument + ". Associated attribute has exited scope.");
    }

    String property_macro = AttributeDB::instance()->get_macro_for_attribute(attribute->get_type());
    Vector<Ref<IAttributeParameterType>> types = AttributeDB::instance()->get_parameters_for_macro(property_macro);

    for (const Ref<IAttributeParameterType>& type : types) {
      String outer, inner;
      split_outer_inner(argument, outer, inner);

      Vector<String> value_names = type->get_value_names();
      auto itr = std::find(value_names.begin(), value_names.end(), outer);
      if (itr == value_names.end()) {
        continue;
      }

      Ref<Argument> argument_node = type->create_argument();
      if (!argument_node) {
        return node_new<ParserError>(ERROR, "Failed to create argument node for type " + type->get_type_name());
      }

      target->add_child(argument_node);
      argument_node->build_child<Identifier>(outer);
      Ref<Arguments> inner_arguments = argument_node->build_child<Arguments>();

      if (inner.empty()) {
        return ParserError::OK;
      }

      Vector<String> arguments = split_arguments(inner);
      auto parameters = type->get_arguments();

      for (Size i = 0; i < arguments.size() && i < parameters.size(); i++) {
        Ref<ParserError> error = parse_inner_arguments(arguments[i], inner_arguments, parameters[i]);

        if (error != ParserError::OK) {
          return error;
        }
      }
    }

    return ParserError::OK;
  }

  Ref<ParserError> GodotAttributeArgumentParser::parse_inner_arguments(
      const String& content, Ref<Context> target, const IAttributeParameterType::Argument& parameter) {
    switch (parameter.type) {
      case IAttributeParameterType::ARG_STRING:
        target->build_child<Argument>().with_child<Literal>(content);
        break;
    }

    return ParserError::OK;
  }

}  // namespace GodotObjectCompiler
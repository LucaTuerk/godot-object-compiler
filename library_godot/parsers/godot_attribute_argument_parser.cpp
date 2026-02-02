
#include "godot_attribute_argument_parser.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Ref<ParserError> GodotAttributeArgumentParser::parse_attribute_arguments(const String& content, Ref<Context> target) {
    Ref<Attribute> attribute = weak_attribute.lock();
    if (!attribute) {
      return node_new<ParserError>(ERROR, "Invalid attribute parser for . Associated attribute has exited scope.");
    }

    const Vector<String> arguments = split_arguments(content);

    if (arguments.empty()) {
      return ParserError::OK;
    }

    String property_macro = AttributeDB::instance()->get_macro_for_attribute(attribute->get_type());
    Vector<Ref<IAttributeParameterType>> types = AttributeDB::instance()->get_parameters_for_macro(property_macro);

    if (arguments.size() > types.size()) {
      return node_new<ParserError>(
          ERROR, format("Invalid argument count, expected at most %d but found %d", types.size(), arguments.size()));
    }

    for (const String& argument : arguments) {
      for (const String& single : split_flags(argument)) {
        if (single.empty()) {
          return node_new<ParserError>(
              ERROR, format("Invalid empty sub argument found in argument \"%s\"", argument.c_str()));
        }

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
    if (types.empty()) {
      return node_new<ParserError>(
          ERROR, format("No valid parameter types found for attribute %s", attribute->get_type().c_str()));
    }

    bool no_match = true;

    for (const Ref<IAttributeParameterType>& type : types) {
      String outer, inner;
      split_outer_inner(argument, outer, inner);

      Vector<String> value_names = type->get_value_names();
      auto itr = std::find(value_names.begin(), value_names.end(), outer);
      if (itr == value_names.end()) {
        continue;
      }
      no_match = false;

      Ref<Argument> argument_node = type->create_argument();
      if (!argument_node) {
        return node_new<ParserError>(ERROR, "Failed to create argument node for type " + type->get_return_type());
      }

      target->add_child(argument_node);
      argument_node->build_child<Identifier>(outer);
      Ref<Arguments> inner_arguments = argument_node->build_child<Arguments>();

      if (inner.empty()) {
        return ParserError::OK;
      }

      Vector<String> arguments = split_arguments(inner);
      auto parameters = type->get_arguments();  //

      if (arguments.size() > parameters.size()) {
        return node_new<ParserError>(
            ERROR, format("Invalid inner argument count for \"%s\", expected at most %d but found %d", argument.c_str(),
                       parameters.size(), arguments.size()));
      }

      if (parameters.size() > arguments.size()) {
        Size optional_count = 0;
        for (const auto& parameter : parameters) {
          if (parameter.optional) {
            optional_count++;
          }
        }

        Size diff = parameters.size() - arguments.size();
        if (diff > optional_count) {
          return node_new<ParserError>(ERROR,
              format("Invalid inner argument count %d for \"%s\", expected %d parameters with %d being optional.",
                  arguments.size(), argument.c_str(), parameters.size(), optional_count));
        }
      }

      for (Size i = 0; i < arguments.size() && i < parameters.size(); i++) {
        Ref<ParserError> error = parse_inner_arguments(arguments[i], inner_arguments, parameters[i]);

        if (error != ParserError::OK) {
          error->set_handled();
          return node_new<ParserError>(ERROR, format("Failed to parse argument \"%s\". %s", argument.c_str(),
                                                  attribute->get_type().c_str(), error->message.c_str()));
        }
      }
    }

    if (no_match) {
      return node_new<ParserError>(
          ERROR, format("Failed to find matching argument type with value name \"%s\" for attribute %s",
                     argument.c_str(), attribute->get_type().c_str()));
    }
    return ParserError::OK;
  }

  Ref<ParserError> GodotAttributeArgumentParser::parse_inner_arguments(
      const String& content, Ref<Context> target, const IAttributeParameterType::Argument& parameter) {
    switch (parameter.type) {
      case IAttributeParameterType::ARG_STRING:
        target->build_child<Argument>().with_child<Literal>(content);
        break;
      default:
        PANIC("Unimplemented IAttributeParameterType %d", static_cast<int>(parameter.type));
    }

    return ParserError::OK;
  }

}  // namespace GodotObjectCompiler
//
// Created by luca on 23.01.26.
//

#include "output_transformator.h"

#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

#define ADD_TRANSFORM_CHILDREN(from, into)                               \
  for (Ref<Node> child : *from) {                                        \
    Ref<Writer::IOutputNode> transformed = transform(child);             \
    if (Ref<Node> node = std::dynamic_pointer_cast<Node>(transformed)) { \
      into->add_child(node);                                             \
    }                                                                    \
  }

#define ADD_TEXT_IF_TYPE(type, text) \
  if (p_tree->is<type>()) {            \
    return Writer::Text(text);       \
  }

  Ref<Writer::IOutputNode> OutputTransformator::transform(Ref<Node> p_tree) {
    if (p_tree == nullptr) {
      return Writer::Text("");
    }

    ADD_TEXT_IF_TYPE(Virtual, "virtual")
    ADD_TEXT_IF_TYPE(Static, "static")
    ADD_TEXT_IF_TYPE(Inline, "inline")
    ADD_TEXT_IF_TYPE(Override, "override")
    ADD_TEXT_IF_TYPE(Const, "const")
    ADD_TEXT_IF_TYPE(Mutable, "mutable")
    ADD_TEXT_IF_TYPE(Volatile, "volatile")
    ADD_TEXT_IF_TYPE(Reference, "&")
    ADD_TEXT_IF_TYPE(Pointer, "*")
    ADD_TEXT_IF_TYPE(ConstExpression, "constexpr")
    ADD_TEXT_IF_TYPE(Unsigned, "unsigned")
    ADD_TEXT_IF_TYPE(Signed, "signed")
    ADD_TEXT_IF_TYPE(Short, "short")
    ADD_TEXT_IF_TYPE(Long, "long")

    if (Ref<Namespace> _namespace = p_tree->as<Namespace>()) {
      return Writer::Spaces(
          {Writer::Text("namespace"), Writer::Text(_namespace->name()), transform(_namespace->body())});
    }

    if (Ref<Body> body = p_tree->as<Body>()) {
      Ref<Writer::ListNode> into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(body, into)
      return Writer::NoSep({Writer::NewLine(), Writer::Braces({Writer::NewLine(), Writer::Indent(2, {into})})});
    }

    if (Ref<Parameters> parameters = p_tree->as<Parameters>()) {
      Ref<Writer::ListNode> into = Writer::Params({});

      for (Ref<Node> child : *parameters) {
        Ref<Parameter> parameter = child->as<Parameter>();
        if (parameter) {
          Ref<Writer::IOutputNode> transformed = transform(parameter);
          if (Ref<Node> node = std::dynamic_pointer_cast<Node>(transformed)) {
            into->add_child(node);
          }
        }
      }

      return Writer::Brackets({into});
    }

    if (Ref<Arguments> arguments = p_tree->as<Arguments>()) {
      Ref<Writer::ListNode> into = Writer::Params({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return Writer::Brackets({into});
    }

    if (Ref<Argument> argument = p_tree->as<Argument>()) {
      Ref<Writer::ListNode> into = Writer::NoSep({});
      ADD_TRANSFORM_CHILDREN(argument, into)
      return into;
    }

    if (Ref<TemplateParameters> arguments = p_tree->as<TemplateParameters>()) {
      Ref<Writer::ListNode> into = Writer::Params({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return Writer::Chevrons({into});
    }

    if (Ref<TemplateArguments> arguments = p_tree->as<TemplateArguments>()) {
      Ref<Writer::ListNode> into = Writer::Params({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return Writer::Chevrons({into});
    }

    if (Ref<Parameter> parameter = p_tree->as<Parameter>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      for (Ref<Node> child : *parameter) {
        Ref<Literal> literal = child->as<Literal>();
        if (literal) {
          into->add_child(Writer::Text(" = "));
        }
        Ref<Writer::IOutputNode> transformed = transform(child);
        if (Ref<Node> node = std::dynamic_pointer_cast<Node>(transformed)) {
          into->add_child(node);
        }
      }
      return into;
    }

    if (Ref<Type> type = p_tree->as<Type>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(type, into)
      return into;
    }

    if (Ref<Identifier> identifier = p_tree->as<Identifier>()) {
      Ref<Node> next = identifier->get_next_sibling();
      if (next && (next->is<Parameter>() || next->is<Argument>())) {
        return Writer::NoSep({Writer::Text(identifier->name), transform(next)});
      } else {
        return Writer::Text(identifier->name);
      }
    }

    if (Ref<Writer::IOutputNode> existing = p_tree->as<Writer::IOutputNode>()) {
      Ref<Node> node = std::dynamic_pointer_cast<Node>(existing);
      if (!node) {
        return Writer::Text("");
      }

      Ref<Writer::IOutputNode> output_node = std::dynamic_pointer_cast<Writer::IOutputNode>(node->clone());
      if (!output_node) {
        return Writer::Text("");
      }

      replace_non_output_children(output_node);
      return output_node;
    }

    if (Ref<Literal> literal = p_tree->as<Literal>()) {
      return Writer::Text(literal->content);
    }

    if (Ref<Field> arguments = p_tree->as<Field>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return into;
    }

    if (Ref<Function> function = p_tree->as<Function>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(function, into);
      return into;
    }

    if (Ref<Context> context = p_tree->as<Context>()) {
      Ref<Writer::ListNode> into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(context, into)
      return into;
    }

    return Writer::Text("");
  }

  void OutputTransformator::replace_non_output_children(Ref<Writer::IOutputNode> p_node) {
    if (Ref<Context> context = std::dynamic_pointer_cast<Context>(p_node)) {
      for (Ref<Node> child : *context) {
        Ref<Writer::IOutputNode> output_child = child->as<Writer::IOutputNode>();
        if (output_child) {
          replace_non_output_children(output_child);
        } else {
          Ref<Writer::IOutputNode> transformed = transform(child);
          Ref<Node> casted = std::dynamic_pointer_cast<Node>(transformed);
          if (casted) {
            context->replace_child(child, casted);
          }
        }
      }
    }
  }

}  // namespace GodotObjectCompiler
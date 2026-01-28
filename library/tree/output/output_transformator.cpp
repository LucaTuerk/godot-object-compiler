//
// Created by luca on 23.01.26.
//

#include "output_transformator.h"

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
  if (tree->is<type>()) {            \
    return Writer::Text(text);       \
  }

  Ref<Writer::IOutputNode> OutputTransformator::transform(Ref<Node> tree) {
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

    if (Ref<Namespace> _namespace = tree->as<Namespace>()) {
      return Writer::Spaces(
          {Writer::Text("namespace"), Writer::Text(_namespace->name()), transform(_namespace->body())});
    }

    if (Ref<Body> body = tree->as<Body>()) {
      Ref<Writer::ListNode> into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(body, into)
      return Writer::NoSep(
          {Writer::NewLine(), Writer::Braces({Writer::NewLine(), Writer::Indent(2, {Writer::NewLine(), into})})});
    }

    if (Ref<Parameters> parameters = tree->as<Parameters>()) {
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

    if (Ref<Arguments> arguments = tree->as<Arguments>()) {
      Ref<Writer::ListNode> into = Writer::Params({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return Writer::Brackets({into});
    }

    if (Ref<Argument> argument = tree->as<Argument>()) {
      Ref<Writer::ListNode> into = Writer::NoSep({});
      ADD_TRANSFORM_CHILDREN(argument, into)
      return into;
    }

    if (Ref<Parameter> parameter = tree->as<Parameter>()) {
      Ref<Writer::ListNode> into = Writer::NoSep({});
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

    if (Ref<Type> type = tree->as<Type>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(type, into)
      return into;
    }

    if (Ref<Identifier> identifier = tree->as<Identifier>()) {
      Ref<Node> next = identifier->get_next_sibling();
      if (next && (next->is<Parameter>() || next->is<Argument>())) {
        return Writer::NoSep({Writer::Text(identifier->name), transform(next)});
      } else {
        return Writer::Text(identifier->name);
      }
    }

    if (Ref<Writer::IOutputNode> existing = tree->as<Writer::IOutputNode>()) {
      Ref<Node> node = std::dynamic_pointer_cast<Node>(existing);
      if (!node) {
        return Writer::Text("");
      }

      Ref<Writer::IOutputNode> output_node = std::dynamic_pointer_cast<Writer::IOutputNode>(node->clone());
      if (!output_node) {
        return Writer::Text("");
      }

      return output_node;
    }

    if (Ref<Literal> literal = tree->as<Literal>()) {
      return Writer::Text(literal->content);
    }

    if (Ref<Field> arguments = tree->as<Field>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return into;
    }

    if (Ref<Function> function = tree->as<Function>()) {
      Ref<Writer::ListNode> into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(function, into);
      return into;
    }

    if (Ref<Context> context = tree->as<Context>()) {
      Ref<Writer::ListNode> into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(context, into)
      return into;
    }

    return Writer::Text("");
  }

}  // namespace GodotObjectCompiler
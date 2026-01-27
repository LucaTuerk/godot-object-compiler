//
// Created by luca on 23.01.26.
//

#include "output_transformator.h"

#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

#define ADD_TRANSFORM_CHILDREN(from, into)               \
  for (Node* child : *from) {                            \
    Writer::IOutputNode* transformed = transform(child); \
    if (Node* node = dynamic_cast<Node*>(transformed)) { \
      into->add_child(node);                             \
    }                                                    \
  }

#define ADD_TEXT_IF_TYPE(type, text) \
  if (tree->is<type>()) {            \
    return Writer::Text(text);       \
  }

  Writer::IOutputNode* OutputTransformator::transform(Node* tree) {
    ADD_TEXT_IF_TYPE(Virtual, "virtual")
    ADD_TEXT_IF_TYPE(Static, "static")
    ADD_TEXT_IF_TYPE(Inline, "inline")
    ADD_TEXT_IF_TYPE(Override, "override")
    ADD_TEXT_IF_TYPE(Const, "const")
    ADD_TEXT_IF_TYPE(Mutable, "mutable")
    ADD_TEXT_IF_TYPE(Volatile, "volatile")
    ADD_TEXT_IF_TYPE(Reference, "&")
    ADD_TEXT_IF_TYPE(Pointer, "*")

    if (Namespace* _namespace = tree->as<Namespace>()) {
      return Writer::Spaces(
          {Writer::Text("namespace"), Writer::Text(_namespace->name()), transform(_namespace->body())});
    }

    if (Body* body = tree->as<Body>()) {
      Writer::ListNode* into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(body, into)
      return Writer::NoSep(
          {Writer::NewLine(), Writer::Braces({Writer::NewLine(), Writer::Indent(2, {Writer::NewLine(), into})})});
    }

    if (Parameters* parameters = tree->as<Parameters>()) {
      Writer::ListNode* into = Writer::Params({});

      for (Node* child : *parameters) {
        Parameter* parameter = child->as<Parameter>();
        if (parameter) {
          Writer::IOutputNode* transformed = transform(parameter);
          if (Node* node = dynamic_cast<Node*>(transformed)) {
            into->add_child(node);
          }
        }
      }

      return Writer::Brackets({into});
    }

    if (Arguments* arguments = tree->as<Arguments>()) {
      Writer::ListNode* into = Writer::Params({});
      ADD_TRANSFORM_CHILDREN(arguments, into)
      return Writer::Brackets({into});
    }

    if (Argument* argument = tree->as<Argument>()) {
      Writer::ListNode* into = Writer::NoSep({});
      ADD_TRANSFORM_CHILDREN(argument, into)
      return into;
    }

    if (Parameter* parameter = tree->as<Parameter>()) {
      Writer::ListNode* into = Writer::NoSep({});
      ADD_TRANSFORM_CHILDREN(parameter, into)
      return into;
    }

    if (Type* type = tree->as<Type>()) {
      Writer::ListNode* into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(type, into)
      return into;
    }

    if (Identifier* identifier = tree->as<Identifier>()) {
      Node* next = identifier->get_next_sibling();
      if (next && (next->is<Parameter>() || next->is<Argument>())) {
        return Writer::NoSep({Writer::Text(identifier->name), transform(next)});
      } else {
        return Writer::Text(identifier->name);
      }
    }

    if (Writer::IOutputNode* existing = tree->as<Writer::IOutputNode>()) {
      Node* node = dynamic_cast<Node*>(existing);
      if (!node) {
        return Writer::Text("");
      }

      Writer::IOutputNode* output_node = dynamic_cast<Writer::IOutputNode*>(node->clone());
      if (!output_node) {
        return Writer::Text("");
      }

      return output_node;
    }

    if (Function* function = tree->as<Function>()) {
      Writer::ListNode* into = Writer::Spaces({});
      ADD_TRANSFORM_CHILDREN(function, into);
      return into;
    }

    if (Context* context = tree->as<Context>()) {
      Writer::ListNode* into = Writer::Lines({});
      ADD_TRANSFORM_CHILDREN(context, into)
      return into;
    }

    return Writer::Text("");
  }

}  // namespace GodotObjectCompiler
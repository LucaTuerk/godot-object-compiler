/**************************************************************************/
/* output_transformator.cpp                                               */
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
  if (p_tree->is<type>()) {          \
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

    if (Ref<Class> _class = p_tree->as<Class>()) {
      String specifier = _class->is<Struct>() ? "struct" : "class";

      auto base_names = _class->direct_bases_names();
      if (base_names.size() == 1) {
        return Writer::Spaces({Writer::Text(specifier), Writer::Text(_class->name()),
            Writer::Text(format(" : public %s", base_names[0].c_str())), transform(_class->body()),
            Writer::Semicolon()});
      }
      ERR_COND(!base_names.empty(), "Classes with multiple inheritance are not supported by the OutputTransformator");
      return Writer::Spaces(
          {Writer::Text(specifier), Writer::Text(_class->name()), transform(_class->body()), Writer::Semicolon()});
    }

    if (Ref<Namespace> _namespace = p_tree->as<Namespace>()) {
      if (_namespace->name().empty()) {
        Ref<Writer::ListNode> into = Writer::Lines({});
        ERR_COND(_namespace->body() == nullptr, "Failed to get global namespace body.");

        ADD_TRANSFORM_CHILDREN(_namespace->body(), into);
        return into;
      }

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

      for (const Ref<Node>& child : *parameters) {
        if (Ref<Parameter> parameter = child->as<Parameter>()) {
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
      for (const Ref<Node>& child : *parameter) {
        if (Ref<Literal> literal = child->as<Literal>()) {
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

  void OutputTransformator::replace_non_output_children(const Ref<Writer::IOutputNode>& p_node) {
    if (const Ref<Context> context = std::dynamic_pointer_cast<Context>(p_node)) {
      for (const Ref<Node>& child : *context) {
        if (Ref<Writer::IOutputNode> output_child = child->as<Writer::IOutputNode>()) {
          replace_non_output_children(output_child);
        } else {
          Ref<Writer::IOutputNode> transformed = transform(child);
          if (Ref<Node> casted = std::dynamic_pointer_cast<Node>(transformed)) {
            context->replace_child(child, casted);
          }
        }
      }
    }
  }

}
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

namespace GodotObjectCompiler
{

#define ADD_TRANSFORM_CHILDREN(from, into)                                                         \
    for (Ref<Node> child : *from) {                                                                \
        Ref<Output::OutputNode> transformed = transform(child);                                    \
        into->add_child(transformed);                                                              \
    }

#define ADD_TEXT_IF_TYPE(type, text)                                                               \
    if (p_tree->is<type>()) {                                                                      \
        return Output::Text(text);                                                                 \
    }

    Ref<Output::OutputNode> OutputTransformator::transform(Ref<Node> p_tree)
    {
        if (p_tree == nullptr) {
            return Output::Text("");
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
                return Output::Spaces(
                    {Output::Text(specifier), Output::Text(_class->name()),
                     Output::Text(format(" : public %s", base_names[0].c_str())),
                     transform(_class->body()), Output::Semicolon()});
            }
            ERR_COND(
                !base_names.empty(),
                "Classes with multiple inheritance are not supported by the OutputTransformator");
            return Output::Spaces(
                {Output::Text(specifier), Output::Text(_class->name()), transform(_class->body()),
                 Output::Semicolon()});
        }

        if (Ref<Namespace> _namespace = p_tree->as<Namespace>()) {
            if (_namespace->name().empty()) {
                Ref<Output::ListNode> into = Output::Lines({});
                ERR_COND(_namespace->body() == nullptr, "Failed to get global namespace body.");

                ADD_TRANSFORM_CHILDREN(_namespace->body(), into);
                return into;
            }

            return Output::Spaces(
                {Output::Text("namespace"), Output::Text(_namespace->name()),
                 transform(_namespace->body())});
        }

        if (Ref<AccessSpecifier> access_specifier = p_tree->as<AccessSpecifier>()) {
            switch (access_specifier->type) {
            case AccessSpecifier::PUBLIC:
                return Output::Text("public:");
            case AccessSpecifier::PRIVATE:
                return Output::Text("private:");
            case AccessSpecifier::PROTECTED:
                return Output::Text("protected:");
            }
        }

        if (Ref<Body> body = p_tree->as<Body>()) {
            Ref<Output::ListNode> into = Output::Lines({});
            ADD_TRANSFORM_CHILDREN(body, into)
            return Output::NoSep(
                {Output::NewLine(),
                 Output::Braces({Output::NewLine(), Output::Indent(2, {into})})});
        }

        if (Ref<Parameters> parameters = p_tree->as<Parameters>()) {
            Ref<Output::ListNode> into = Output::Params({});

            for (const Ref<Node>& child : *parameters) {
                if (Ref<Parameter> parameter = child->as<Parameter>()) {
                    Ref<Output::OutputNode> transformed = transform(parameter);
                    if (Ref<Node> node = std::dynamic_pointer_cast<Node>(transformed)) {
                        into->add_child(node);
                    }
                }
            }

            return Output::Brackets({into});
        }

        if (Ref<Arguments> arguments = p_tree->as<Arguments>()) {
            Ref<Output::ListNode> into = Output::Params({});
            ADD_TRANSFORM_CHILDREN(arguments, into)
            return Output::Brackets({into});
        }

        if (Ref<Argument> argument = p_tree->as<Argument>()) {
            Ref<Output::ListNode> into = Output::NoSep({});
            ADD_TRANSFORM_CHILDREN(argument, into)
            return into;
        }

        if (Ref<TemplateParameters> arguments = p_tree->as<TemplateParameters>()) {
            Ref<Output::ListNode> into = Output::Params({});
            ADD_TRANSFORM_CHILDREN(arguments, into)
            return Output::Chevrons({into});
        }

        if (Ref<TemplateArguments> arguments = p_tree->as<TemplateArguments>()) {
            Ref<Output::ListNode> into = Output::Params({});
            ADD_TRANSFORM_CHILDREN(arguments, into)
            return Output::Chevrons({into});
        }

        if (Ref<Parameter> parameter = p_tree->as<Parameter>()) {
            Ref<Output::ListNode> into = Output::Spaces({});
            for (const Ref<Node>& child : *parameter) {
                if (Ref<Literal> literal = child->as<Literal>()) {
                    into->add_child(Output::Text(" = "));
                }
                Ref<Output::OutputNode> transformed = transform(child);
                into->add_child(transformed);
            }
            return into;
        }

        if (Ref<Type> type = p_tree->as<Type>()) {
            Ref<Output::ListNode> into = Output::Spaces({});
            ADD_TRANSFORM_CHILDREN(type, into)
            return into;
        }

        if (Ref<Identifier> identifier = p_tree->as<Identifier>()) {
            Ref<Node> next = identifier->get_next_sibling();
            if (next && (next->is<Parameter>() || next->is<Argument>())) {
                return Output::NoSep({Output::Text(identifier->name), transform(next)});
            } else {
                return Output::Text(identifier->name);
            }
        }

        if (Ref<Output::OutputNode> existing = p_tree->as<Output::OutputNode>()) {
            Ref<Output::OutputNode> output_node = existing->clone<Output::OutputNode>();
            if (!output_node) {
                return Output::Text("");
            }

            replace_non_output_children(existing);
            return output_node;
        }

        if (Ref<Literal> literal = p_tree->as<Literal>()) {
            return Output::Text(literal->content);
        }

        if (Ref<Field> arguments = p_tree->as<Field>()) {
            Ref<Output::ListNode> into = Output::Spaces({});
            ADD_TRANSFORM_CHILDREN(arguments, into)
            return into;
        }

        if (Ref<Function> function = p_tree->as<Function>()) {
            Ref<Output::ListNode> into = Output::Spaces({});
            ADD_TRANSFORM_CHILDREN(function, into);
            return into;
        }

        if (Ref<Context> context = p_tree->as<Context>()) {
            Ref<Output::ListNode> into = Output::Lines({});
            ADD_TRANSFORM_CHILDREN(context, into)
            return into;
        }

        return Output::Text("");
    }

    void OutputTransformator::replace_non_output_children(const Ref<Output::OutputNode>& p_node)
    {
        if (const Ref<Context> context = std::dynamic_pointer_cast<Context>(p_node)) {
            for (const Ref<Node>& child : *context) {
                if (Ref<Output::OutputNode> output_child = child->as<Output::OutputNode>()) {
                    replace_non_output_children(output_child);
                } else {
                    Ref<Output::OutputNode> transformed = transform(child);
                    if (Ref<Node> casted = std::dynamic_pointer_cast<Node>(transformed)) {
                        context->replace_child(child, casted);
                    }
                }
            }
        }
    }

} // namespace GodotObjectCompiler
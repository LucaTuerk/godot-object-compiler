/**************************************************************************/
/* output.cpp                                                             */
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

#include "output.h"

#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "output_transformator.h"

#define ADD_CHILDREN_AND_RET(creator)                                                              \
    auto result = LibraryContext::instance()->get_node_db()->creator;                              \
    for (const Ref<Node>& child : p_children) {                                                    \
        result->add_child(child);                                                                  \
    }                                                                                              \
    return result

namespace GodotObjectCompiler
{
    namespace Output
    {

        void IndentNode::get_output(IStringWriter* p_writer)
        {
            auto child_writer = StreamWriter();

            for (const Ref<Node>& child : get_children()) {
                if (Ref<OutputNode> output = child->as<OutputNode>()) {
                    output->get_output(&child_writer);
                } else {
                    OutputTransformator transformator;
                    transformator.transform(child)->get_output(&child_writer);
                }
            }

            Size total = total_amount();
            String child_result = child_writer.get_string();
            Vector<String> lines = string_split(child_result, "\n");
            for (const String& line : lines) {
                for (Size i = 0; i < total; ++i) {
                    p_writer->write(" ");
                }
                if (!line.empty()) {
                    p_writer->write(line);
                }
                p_writer->write("\n");
            }
        }

        bool IndentNode::copy_to(const Ref<Node>& p_other) const
        {
            COPY_GUARD(IndentNode, Context)

            target->amount = amount;
            return true;
        }

        Size IndentNode::_total_amount_lazy_get() const
        {
            if (Ref<IndentNode> parent = find_ancestor<IndentNode>()) {
                return parent->total_amount() + amount;
            }

            return amount;
        }

        void EnclosingNode::get_output(IStringWriter* p_writer)
        {
            p_writer->write(before);
            for (const Ref<Node>& child : get_children()) {
                if (const Ref<OutputNode> child_output = child->as<OutputNode>()) {
                    child_output->get_output(p_writer);
                } else {
                    OutputTransformator transformator;
                    transformator.transform(child)->get_output(p_writer);
                }
            }
            p_writer->write(after);
        }

        bool EnclosingNode::copy_to(const Ref<Node>& p_other) const
        {
            COPY_GUARD(EnclosingNode, Context)

            target->before = before;
            target->after = after;
            return true;
        }

        void ListNode::get_output(IStringWriter* p_writer)
        {
            if (get_child_count() == 0) {
                return;
            }

            bool last_empty = false;

            for (Index i = 0; i < get_child_count(); ++i) {
                if ((i != 0 && !last_empty) || before_first) {
                    p_writer->write(delimiter);
                }

                const Ref<Node> child = get_child(i);

                StreamWriter child_writer;
                if (Ref<OutputNode> output = child->as<OutputNode>()) {
                    output->get_output(&child_writer);
                } else {
                    OutputTransformator transformator;
                    transformator.transform(child)->get_output(&child_writer);
                }

                last_empty =
                    child_writer.current_length() == 0 || child_writer.get_string() == delimiter;
                p_writer->write(child_writer.get_string());
            }

            if (after_last) {
                p_writer->write(delimiter);
            }

            Brackets({});
        }

        bool ListNode::copy_to(const Ref<Node>& p_other) const
        {
            COPY_GUARD(ListNode, Context)

            target->before_first = before_first;
            target->after_last = after_last;
            target->delimiter = delimiter;
            return true;
        }

        void ReplaceNode::get_output(IStringWriter* p_writer)
        {
            for (const Ref<Node>& child : *this) {
                StreamWriter child_writer;
                if (Ref<OutputNode> output_node = child->as<OutputNode>()) {
                    output_node->get_output(&child_writer);
                } else {
                    OutputTransformator transformator;
                    transformator.transform(child)->get_output(&child_writer);
                }
                p_writer->write(string_replace(child_writer.get_string(), search, replace));
            }
        }

        bool ReplaceNode::copy_to(const Ref<Node>& p_other) const
        {
            COPY_GUARD(ReplaceNode, Context)

            target->search = search;
            target->replace = replace;
            return true;
        }

        void SnippetNode::get_output(IStringWriter* p_writer)
        {
            p_writer->write(content);
        }

        bool SnippetNode::copy_to(const Ref<Node>& p_other) const
        {
            COPY_GUARD(SnippetNode, Node)

            target->content = content;
            return true;
        }

        Ref<IndentNode> Indent(Size p_indent, std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<IndentNode>(p_indent));
        }

        Ref<EnclosingNode> Brackets(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<EnclosingNode>("(", ")"));
        }

        Ref<EnclosingNode> Braces(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<EnclosingNode>("{", "}"));
        }

        Ref<EnclosingNode> Chevrons(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<EnclosingNode>("<", ">"));
        }

        Ref<ListNode> Lines(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<ListNode>("\n", false, false););
        }

        Ref<ReplaceNode> EscapedLines(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<ReplaceNode>("\n", "\\\n"););
        }

        Ref<ListNode> Spaces(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<ListNode>(" ", false, false););
        }

        Ref<ListNode> NoSep(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<ListNode>("", false, false););
        }

        Ref<ListNode> Params(std::initializer_list<Ref<Node>>&& p_children)
        {
            ADD_CHILDREN_AND_RET(create<ListNode>(", ", false, false););
        }

        Ref<ListNode> LineOfCode(std::initializer_list<Ref<Node>>&& p_children)
        {
            return NoSep({NoSep({std::move(p_children)}), Semicolon()});
        }

        Ref<SnippetNode> Text(const String& p_content)
        {
            return LibraryContext::instance()->get_node_db()->create<SnippetNode>(p_content);
        }

        Ref<SnippetNode> BoldText(const String& p_content)
        {
            return LibraryContext::instance()->get_node_db()->create<SnippetNode>(
                format("<b>%s</b>", p_content.c_str()));
        }

        Ref<SnippetNode> StringLiteral(const String& p_content)
        {
            return LibraryContext::instance()->get_node_db()->create<SnippetNode>(
                "\"" + p_content + "\"");
        }

        Ref<SnippetNode> Semicolon()
        {
            return LibraryContext::instance()->get_node_db()->create<SnippetNode>(";");
        }

        Ref<SnippetNode> NewLine()
        {
            return LibraryContext::instance()->get_node_db()->create<SnippetNode>("\n");
        }

        Ref<ListNode> Assign(const String& p_variable_name, const Ref<Node>& p_value)
        {
            return NoSep({Spaces({Text(p_variable_name), Text("="), p_value}), Semicolon()});
        }

        Ref<ListNode> Return(const String& p_name)
        {
            return LineOfCode({Spaces({Text("return"), Text(p_name)})});
        }

        Ref<SnippetNode> Include(const Path& p_path)
        {
            return node_new<SnippetNode>("#include \"" + String(p_path) + "\"");
        }

        Ref<SnippetNode> SystemInclude(const Path& p_path)
        {
            return node_new<SnippetNode>("#include <" + String(p_path) + ">");
        }

        Ref<EnclosingNode> DocComment(const Ref<Node>& p_content)
        {
            return B<EnclosingNode>(
                "/**\n*", "/")[B<ReplaceNode>("\n", "\n*")[B<IndentNode>(2)[p_content]]];
        }

        Ref<ListNode> Define(
            const String& p_name, std::initializer_list<Ref<Node>>&& p_params,
            const String& p_content)
        {
            return Spaces(
                {Text("#define"), NoSep({Text(p_name), Brackets({Params(std::move(p_params))})}),
                 EscapedLines({Text(p_content)}), NewLine(), NewLine()});
        }

        Ref<ListNode> Define(
            const String& p_name, std::initializer_list<Ref<Node>>&& p_params,
            std::initializer_list<Ref<Node>>&& p_lines)
        {
            return Spaces(
                {Text("#define"), NoSep({Text(p_name), Brackets({Params(std::move(p_params))})}),
                 EscapedLines({std::move(p_lines)}), NewLine(), NewLine()});
        }

        Ref<SnippetNode> PragmaOnce()
        {
            return node_new<SnippetNode>("#pragma once\n\n");
        }

        Ref<ListNode> If(const Ref<Node>& p_condition, std::initializer_list<Ref<Node>> p_children)
        {
            Ref<ListNode> result = node_new<ListNode>();
            result->add_children({
                Text("if("),
                p_condition,
                Text(")"),
                B<Body>()[std::move(p_children)],
            });
            return result;
        }

    } // namespace Output
} // namespace GodotObjectCompiler

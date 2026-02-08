/**************************************************************************/
/* output.h                                                               */
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
#pragma once

#include <utility>

#include "../syntax/all.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class IStringWriter;

  namespace Writer {

    class IOutputNode {
     public:

      virtual void get_output(IStringWriter* p_writer) = 0;
    };

    class IndentNode : public Context, public IOutputNode {
      NODE_TYPE(IndentNode)
      LAZY(IndentNode, Size, total_amount);

     public:

      explicit IndentNode(Size amount) : amount(amount) {}

      void get_output(IStringWriter* p_writer) override;

      bool copy_to(Ref<Node> p_other) const override;

      Size amount = 0;
    };

    class EnclosingNode : public Context, public IOutputNode {
      NODE_TYPE(EnclosingNode)

      EnclosingNode(String before, String after) : before(std::move(before)), after(std::move(after)) {}

      void get_output(IStringWriter* p_writer) override;
      bool copy_to(Ref<Node> p_other) const override;

      String before;
      String after;
    };

    class ListNode : public Context, public IOutputNode {
      NODE_TYPE(ListNode)

      ListNode(String delimiter, bool before_first, bool after_last)
          : delimiter(std::move(delimiter)), before_first(before_first), after_last(after_last) {}

      void get_output(IStringWriter* p_writer) override;

      bool copy_to(Ref<Node> p_other) const override;

      String delimiter;
      bool before_first = false;
      bool after_last = true;
    };

    class ReplaceNode : public Context, public IOutputNode {
      NODE_TYPE(ReplaceNode)

      explicit ReplaceNode(const String& search, const String& replace) : search(search), replace(replace) {}

      void get_output(IStringWriter* p_writer) override;

      bool copy_to(Ref<Node> p_other) const override;

      String search;
      String replace;
    };

    class SnippetNode : public Node, public IOutputNode {
      NODE_TYPE(SnippetNode);

      explicit SnippetNode(const String& content) : content(content) {}

      void get_output(IStringWriter* p_writer) override;

      bool copy_to(Ref<Node> p_other) const override;

      String content;
    };

    Ref<IndentNode> Indent(Size p_indent, std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<EnclosingNode> Brackets(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<EnclosingNode> SquareBrackets(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<EnclosingNode> Braces(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<EnclosingNode> Chevrons(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> Lines(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ReplaceNode> EscapedLines(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> Spaces(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> NoSep(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> Params(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> ConstRefParam(const String& p_type, const String& p_name, Ref<IOutputNode> p_default_val = nullptr);

    Ref<ListNode> Param(const String& p_type, const String& p_name, Ref<IOutputNode> p_default_val = nullptr);

    Ref<ListNode> LineOfCode(std::initializer_list<Ref<IOutputNode>>&& p_children);

    Ref<ListNode> ParamDecl(const String& p_type, const String& p_name);

    Ref<SnippetNode> Text(const String& p_content);

    Ref<SnippetNode> BoldText(const String& p_content);

    Ref<SnippetNode> StringLiteral(const String& p_content);

    Ref<SnippetNode> Semicolon();

    Ref<SnippetNode> NewLine();

    Ref<ListNode> FuncCall(const String& p_function_name, std::initializer_list<Ref<IOutputNode>>&& p_parameters);

    Ref<ListNode> FuncImpl(const String& p_modifiers_front, const String& p_return_type, const String& p_function_name,
        std::initializer_list<Ref<IOutputNode>>&& p_params, const String& p_modifiers,
        std::initializer_list<Ref<IOutputNode>>&& p_lines);

    Ref<ListNode> FuncDef(const String& p_modifiers_front, const String& p_return_type, const String& p_function_name,
        std::initializer_list<Ref<IOutputNode>>&& p_params, const String& p_modifiers);

    Ref<ListNode> ConstRef(const String& p_type);

    Ref<ListNode> MemberFuncDef(const String& p_type, const String& p_name,
        std::initializer_list<Ref<IOutputNode>>&& p_parameters, const String& p_modifiers);

    Ref<ListNode> MemberFuncImpl(const String& p_return_type, const String& p_class_name, const String& p_name,
        std::initializer_list<Ref<IOutputNode>>&& p_params, const String& modifiers,
        std::initializer_list<Ref<IOutputNode>>&& lines);

    Ref<ListNode> DeclAssign(const String& p_type, const String& p_name, Ref<IOutputNode> p_value);

    Ref<ListNode> Assign(const String& p_variable_name, Ref<IOutputNode> p_value);

    Ref<ListNode> Return(const String& p_name);

    Ref<SnippetNode> Param(const String& p_name);

    Ref<SnippetNode> Include(const String& p_path);

    Ref<SnippetNode> SystemInclude(const String& p_path);

    Ref<ListNode> Namespace(const String& p_name, Ref<IOutputNode> p_content);

    Ref<ListNode> Class(const String& p_name, Ref<IOutputNode> p_content);

    Ref<ListNode> Class(const String& p_name, const String& p_base, Ref<IOutputNode> p_content);

    Ref<ListNode> Enum(const String& p_name, Ref<IOutputNode> p_content);

    Ref<ListNode> MacroDefine(const String& p_name, Ref<IOutputNode> p_content);

    Ref<ListNode> MacroFunctionDefine(const String& p_name, std::initializer_list<Ref<IOutputNode>> p_params,
        std::initializer_list<Ref<IOutputNode>> p_lines);

    Ref<EnclosingNode> DocComment(Ref<Node> p_content);

    Ref<ListNode> Define(
        const String& p_name, std::initializer_list<Ref<IOutputNode>> p_params, const String& p_content);

    Ref<ListNode> Define(const String& p_name, std::initializer_list<Ref<IOutputNode>> p_params,
        std::initializer_list<Ref<IOutputNode>>&& p_lines);

    Ref<SnippetNode> PragmaOnce();

  }  // namespace Writer
}  // namespace GodotObjectCompiler

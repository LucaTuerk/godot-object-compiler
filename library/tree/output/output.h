#pragma once

#include <utility>

#include "../syntax/all.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class IStringWriter;

  namespace Writer {

    class IOutputNode {
     public:

      virtual void get_output(IStringWriter* writer) = 0;
    };

    class IndentNode : public Context, public IOutputNode {
      NODE_TYPE(IndentNode)
      LAZY(IndentNode, Size, total_amount);

     public:

      explicit IndentNode(Size amount) : amount(amount) {}

      void get_output(IStringWriter* writer) override;

      bool copy_to(Ref<Node> other) const override;

      Size amount = 0;
    };

    class EnclosingNode : public Context, public IOutputNode {
      NODE_TYPE(EnclosingNode)

      EnclosingNode(String before, String after) : before(std::move(before)), after(std::move(after)) {}

      void get_output(IStringWriter* writer) override;
      bool copy_to(Ref<Node> other) const override;

      String before;
      String after;
    };

    class ListNode : public Context, public IOutputNode {
      NODE_TYPE(ListNode)

      ListNode(String delimiter, bool before_first, bool after_last)
          : delimiter(std::move(delimiter)), before_first(before_first), after_last(after_last) {}

      void get_output(IStringWriter* writer) override;

      bool copy_to(Ref<Node> other) const override;

      String delimiter;
      bool before_first = false;
      bool after_last = true;
    };

    class ReplaceNode : public Context, public IOutputNode {
      NODE_TYPE(ReplaceNode)

      explicit ReplaceNode(const String& search, const String& replace) : search(search), replace(replace) {}

      void get_output(IStringWriter* writer) override;

      bool copy_to(Ref<Node> other) const override;

      String search;
      String replace;
    };

    class SnippetNode : public Node, public IOutputNode {
      NODE_TYPE(SnippetNode);

      explicit SnippetNode(const String& content) : content(content) {}

      void get_output(IStringWriter* writer) override;

      bool copy_to(Ref<Node> other) const override;

      String content;
    };

    Ref<IndentNode> Indent(Size indent, std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<EnclosingNode> Brackets(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<EnclosingNode> SquareBrackets(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<EnclosingNode> Braces(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<EnclosingNode> Chevrons(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> Lines(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ReplaceNode> EscapedLines(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> Spaces(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> NoSep(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> Params(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> ConstRefParam(const String& type, const String& name, Ref<IOutputNode> default_val = nullptr);
    Ref<ListNode> Param(const String& type, const String& name, Ref<IOutputNode> default_val = nullptr);
    Ref<ListNode> LineOfCode(std::initializer_list<Ref<IOutputNode>>&& children);
    Ref<ListNode> ParamDecl(const String& type, const String& name);
    Ref<SnippetNode> Text(const String& content);
    Ref<SnippetNode> BoldText(const String& content);
    Ref<SnippetNode> StringLiteral(const String& content);
    Ref<SnippetNode> Semicolon();
    Ref<SnippetNode> NewLine();
    Ref<ListNode> FuncCall(const String& function_name, std::initializer_list<Ref<IOutputNode>>&& parameters);

    Ref<ListNode> FuncImpl(const String& modifiers_front, const String& return_type, const String& function_name,
        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers,
        std::initializer_list<Ref<IOutputNode>>&& lines);

    Ref<ListNode> FuncDef(const String& modifiers_front, const String& return_type, const String& function_name,

        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers);

    Ref<ListNode> ConstRef(const String& type);
    Ref<ListNode> MemberFuncDef(const String& type, const String& name,
        std::initializer_list<Ref<IOutputNode>>&& parameters, const String& modifiers);
    Ref<ListNode> MemberFuncImpl(const String& return_type, const String& class_name, const String& name,
        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers,
        std::initializer_list<Ref<IOutputNode>>&& lines);
    Ref<ListNode> DeclAssign(const String& type, const String& name, Ref<IOutputNode> value);
    Ref<ListNode> Assign(const String& variable_name, Ref<IOutputNode> value);
    Ref<ListNode> Return(const String& name);
    Ref<SnippetNode> Param(const String& name);
    Ref<SnippetNode> Include(const String& path);
    Ref<ListNode> Namespace(const String& name, Ref<IOutputNode> content);
    Ref<ListNode> Class(const String& name, Ref<IOutputNode> content);
    Ref<ListNode> Class(const String& name, const String& base, Ref<IOutputNode> content);
    Ref<ListNode> Enum(const String& name, Ref<IOutputNode> content);
    Ref<ListNode> MacroDefine(const String& name, Ref<IOutputNode> content);
    Ref<ListNode> MacroFunctionDefine(const String& name, std::initializer_list<Ref<IOutputNode>> params,
        std::initializer_list<Ref<IOutputNode>> lines);

    Ref<EnclosingNode> DocComment(Ref<Node> content);

    Ref<ListNode> Define(const String& name, std::initializer_list<Ref<IOutputNode>> params, const String& content);
    Ref<ListNode> Define(const String& name, std::initializer_list<Ref<IOutputNode>> params,
        std::initializer_list<Ref<IOutputNode>>&& lines);
    Ref<SnippetNode> PragmaOnce();

  }  // namespace Writer
}  // namespace GodotObjectCompiler

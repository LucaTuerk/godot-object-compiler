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
      Size amount = 0;
    };

    class EnclosingNode : public Context, public IOutputNode {
      NODE_TYPE(EnclosingNode)
      EnclosingNode(String before, String after) : before(std::move(before)), after(std::move(after)) {}

      void get_output(IStringWriter* writer) override;

      String before;
      String after;
    };

    class ListNode : public Context, public IOutputNode {
      NODE_TYPE(ListNode)
      ListNode(String delimiter, bool before_first, bool after_last)
          : delimiter(std::move(delimiter)), before_first(before_first), after_last(after_last) {}

      void get_output(IStringWriter* writer) override;

      String delimiter;
      bool before_first = false;
      bool after_last = true;
    };

    class SnippetNode : public Node, public IOutputNode {
      NODE_TYPE(SnippetNode);

     public:
      explicit SnippetNode(const String& content) : content(content) {}
      void get_output(IStringWriter* writer) override;
      String content;
    };

    void NamespaceOpen(Context*& target, const String& name);

    IndentNode* Indent(Size indent, std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Brackets(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* SquareBrackets(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Braces(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Chevrons(std::initializer_list<IOutputNode*>&& children);
    ListNode* Lines(std::initializer_list<IOutputNode*>&& children);
    ListNode* EscapedLines(std::initializer_list<IOutputNode*>&& children);
    ListNode* Spaces(std::initializer_list<IOutputNode*>&& children);
    ListNode* NoSep(std::initializer_list<IOutputNode*>&& children);
    ListNode* Params(std::initializer_list<IOutputNode*>&& children);
    ListNode* ConstRefParam(const String& type, const String& name, IOutputNode* default_val = nullptr);
    ListNode* Param(const String& type, const String& name, IOutputNode* default_val = nullptr);
    ListNode* LineOfCode(std::initializer_list<IOutputNode*>&& children);
    ListNode* ParamDecl(const String& type, const String& name);
    SnippetNode* Text(const String& content);
    SnippetNode* StringLiteral(const String& content);
    SnippetNode* Semicolon();
    SnippetNode* NewLine();
    ListNode* FuncCall(const String& function_name, std::initializer_list<IOutputNode*>&& parameters);
    ListNode* FuncImpl(const String& return_type, const String& function_name,
                       std::initializer_list<IOutputNode*>&& params, const String& modifiers,
                       std::initializer_list<IOutputNode*>&& lines);
    ListNode* ConstRef(const String& type);
    ListNode* MemberFuncDef(const String& type, const String& name, std::initializer_list<IOutputNode*>&& parameters,
                            const String& modifiers);
    ListNode* MemberFuncImpl(const String& return_type, const String& class_name, const String& name,
                             std::initializer_list<IOutputNode*>&& params, const String& modifiers,
                             std::initializer_list<IOutputNode*>&& lines);
    ListNode* DeclAssign(const String& type, const String& name, IOutputNode* value);
    ListNode* Assign(const String& variable_name, IOutputNode* value);
    ListNode* Return(const String& name);
    SnippetNode* Param(const String& name);
    ListNode* Namespace(const String& name, IOutputNode* content);
    ListNode* Class(const String& name, IOutputNode* content);
    ListNode* Class(const String& name, const String& base, IOutputNode* content);
    ListNode* Enum(const String& name, IOutputNode* content);

    ListNode* MacroDefine(const String& name, IOutputNode* content);
    ListNode* MacroFunctionDefine(const String& name, std::initializer_list<IOutputNode*> params,
                                  std::initializer_list<IOutputNode*> lines);

  }  // namespace Writer
}  // namespace GodotObjectCompiler

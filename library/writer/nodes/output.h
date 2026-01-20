#pragma once

#include <utility>

#include "library/core/core.h"
#include "library/tree/all.h"

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
      EnclosingNode(String before, String after)
          : before(std::move(before)), after(std::move(after)) {}

      void get_output(IStringWriter* writer) override;

      String before;
      String after;
    };

    class ListNode : public Context, public IOutputNode {
      NODE_TYPE(ListNode)
      ListNode(String delimiter, bool before_first, bool after_last)
          : delimiter(std::move(delimiter)),
            before_first(before_first),
            after_last(after_last) {}

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

    IndentNode* Indent(Size indent, std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Brackets(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* SquareBrackets(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Braces(std::initializer_list<IOutputNode*>&& children);
    EnclosingNode* Chevrons(std::initializer_list<IOutputNode*>&& children);
    ListNode* Lines(std::initializer_list<IOutputNode*>&& children);
    ListNode* Spaces(std::initializer_list<IOutputNode*>&& children);
    ListNode* NoSep(std::initializer_list<IOutputNode*>&& children);
    ListNode* Params(std::initializer_list<IOutputNode*>&& children);
    SnippetNode* Text(const String& content);
    SnippetNode* StringLiteral(const String& content);
    SnippetNode* Semicolon();

    ListNode* FuncCall(const String& function_name, std::initializer_list<IOutputNode*>&& parameters);
    SnippetNode* Param(const String& name);

  }  // namespace Writer
}  // namespace GodotObjectCompiler

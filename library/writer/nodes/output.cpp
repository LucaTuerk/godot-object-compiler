#include "output.h"

#include "string_writer.h"

namespace GodotObjectCompiler {
  namespace Writer {

    void IndentNode::get_output(IStringWriter* writer) {
      StringWriter child_writer = StringWriter();

      for (Node* child : get_children()) {
        IOutputNode* output = child->as<IOutputNode>();
        if (output) {
          output->get_output(&child_writer);
        }
      }

      String child_result = child_writer.get_string();
      Size length = child_result.length();
      Size total = total_amount();
      Size start = 0;

      do {
        Size prev_start = start;
        start = child_result.find("\n", start) + 1;
        writer->write(child_result.substr(
            prev_start, start == String::npos ? start : start - prev_start));
        for (Size i = 0; i < total; ++i) {
          writer->write(" ");
        }
      } while (start < length);
    }

    Size IndentNode::_total_amount_lazy_get() {
      IndentNode* parent = find_ancestor<IndentNode>();

      if (parent) {
        return parent->total_amount() + amount;
      }

      return amount;
    }

    void EnclosingNode::get_output(IStringWriter* writer) {
      writer->write(before);
      for (Node* child : get_children()) {
        IOutputNode* child_output = child->as<IOutputNode>();
        if (child_output) {
          child_output->get_output(writer);
        }
      }
      writer->write(after);
    }

    void ListNode::get_output(IStringWriter* writer) {
      if (get_child_count() == 0) {
        return;
      }

      for (Index i = 0; i < get_child_count(); ++i) {
        if (i != 0 || before_first) {
          writer->write(delimiter);
        }

        Node* child = get_child(i);
        IOutputNode* output = child->as<IOutputNode>();
        if (output) {
          output->get_output(writer);
        }
      }

      if (after_last) {
        writer->write(delimiter);
      }

      Brackets({});
    }

    void SnippetNode::get_output(IStringWriter* writer) {
      writer->write(content);
    }

#define ADD_CHILDREN_AND_RET(creator)            \
  auto result = NodeDB::get_instance()->creator; \
  for (IOutputNode* child : children) {          \
    Node* node = dynamic_cast<Node*>(child);     \
    if (node) {                                  \
      result->add_child(node);                   \
    }                                            \
  }                                              \
  return result

    IndentNode* Indent(Size indent,
                       std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<IndentNode>(indent));
    }
    EnclosingNode* Brackets(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("(", ")"));
    }

    EnclosingNode* SquareBrackets(
        std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("[", "]"));
    }

    EnclosingNode* Braces(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("{", "}"));
    }

    EnclosingNode* Chevrons(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("<", ">"));
    }

    ListNode* Lines(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>("\n", true, true););
    }

    ListNode* Spaces(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>(" ", false, false););
    }

    ListNode* NoSep(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>("", false, false););
    }

    ListNode* Params(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>(", ", false, false););
    }

    SnippetNode* Text(const String& content) {
      return NodeDB::get_instance()->create<SnippetNode>(content);
    }

    SnippetNode* StringLiteral(const String& content) {
      return NodeDB::get_instance()->create<SnippetNode>("\"" + content + "\"");
    }

    SnippetNode* Semicolon() {
      return NodeDB::get_instance()->create<SnippetNode>(";");
    }

    ListNode* FuncCall(const String& function_name,
                   std::initializer_list<IOutputNode*>&& parameters) {
      return NoSep({
        Text(function_name),
        Brackets({
          Params(std::move(parameters))
        })
      });
    }

    SnippetNode* Param(const String& name) {
      return NodeDB::get_instance()->create<SnippetNode>(name);
    }

  }  // namespace Writer
}  // namespace GodotObjectCompiler

#include "output.h"

#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {
  namespace Writer {

    void IndentNode::get_output(IStringWriter* writer) {
      StreamWriter child_writer = StreamWriter();

      for (Node* child : get_children()) {
        IOutputNode* output = child->as<IOutputNode>();
        if (output) {
          output->get_output(&child_writer);
        }
      }

      Size total = total_amount();
      String child_result = child_writer.get_string();
      Vector<String> lines = string_split(child_result, "\n");
      for (const String& line : lines) {
        if (!line.empty()) {
          for (Size i = 0; i < total; ++i) {
            writer->write(" ");
          }
          writer->write(line);
          writer->write("\n");
        }
      }
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

      bool last_empty = false;

      for (Index i = 0; i < get_child_count(); ++i) {
        if ((i != 0 && !last_empty) || before_first) {
          writer->write(delimiter);
        }

        Node* child = get_child(i);
        IOutputNode* output = child->as<IOutputNode>();

        if (output) {
          Size tmp = writer->current_length();
          output->get_output(writer);
          last_empty = tmp == writer->current_length();
        }
      }

      if (after_last) {
        writer->write(delimiter);
      }

      Brackets({});
    }

    void SnippetNode::get_output(IStringWriter* writer) { writer->write(content); }

#define ADD_CHILDREN_AND_RET(creator)                                 \
  auto result = ExecutionContext::instance()->get_node_db()->creator; \
  for (IOutputNode* child : children) {                               \
    Node* node = dynamic_cast<Node*>(child);                          \
    if (node) {                                                       \
      result->add_child(node);                                        \
    }                                                                 \
  }                                                                   \
  return result

    void NamespaceOpen(Context*& target, const String& name) {
      IOutputNode* content = Lines({});
      target->add_child(Spaces({Text(name), Braces({NewLine(), Indent(4, {content}), NewLine()})}));
      target = dynamic_cast<Context*>(content);
    }

    IndentNode* Indent(Size indent, std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<IndentNode>(indent));
    }
    EnclosingNode* Brackets(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("(", ")"));
    }

    EnclosingNode* SquareBrackets(std::initializer_list<IOutputNode*>&& children) {
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

    ListNode* EscapedLines(std::initializer_list<IOutputNode*>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>("\\\n", true, false));
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

    ListNode* Param(const String& type, const String& name, IOutputNode* default_val) {
      return default_val ? Spaces({Text(type), Text(name), Text("="), default_val}) : Spaces({Text(type), Text(name)});
    }

    ListNode* ConstRefParam(const String& type, const String& name, IOutputNode* default_val) {
      return default_val ? Spaces({ConstRef(type), Text(name), Text("="), default_val})
                         : Spaces({ConstRef(type), Text(name)});
    }

    ListNode* LineOfCode(std::initializer_list<IOutputNode*>&& children) {
      return NoSep({NoSep({std::move(children)}), Semicolon()});
    }

    SnippetNode* Text(const String& content) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(content);
    }

    SnippetNode* StringLiteral(const String& content) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>("\"" + content + "\"");
    }

    SnippetNode* Semicolon() { return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(";"); }
    SnippetNode* NewLine() { return ExecutionContext::instance()->get_node_db()->create<SnippetNode>("\n"); }

    ListNode* ParamDecl(const String& type, const String& name) {
      return Spaces({
          Text(type),
          Text(name),
      });
    }

    ListNode* FuncImpl(const String& return_type, const String& function_name,
                       std::initializer_list<IOutputNode*>&& params, const String& modifiers,
                       std::initializer_list<IOutputNode*>&& lines) {
      return Spaces({Text(return_type),
                     NoSep({
                         Text(function_name),
                         Brackets({Params(std::move(params))}),
                     }),
                     Text(modifiers),
                     Braces({
                         NewLine(),
                         Indent(2, {Lines(std::move(lines))}),
                     }),
                     NewLine()});
    }
    ListNode* DeclAssign(const String& type, const String& name, IOutputNode* value) {
      return LineOfCode({Spaces({Text(type), Text(name), Text("="), value})});
    }

    ListNode* Assign(const String& variable_name, IOutputNode* value) {
      return NoSep({Spaces({Text(variable_name), Text("="), value}), Semicolon()});
    }

    ListNode* Return(const String& name) { return LineOfCode({Spaces({Text("return"), Text(name)})}); }

    ListNode* FuncCall(const String& function_name, std::initializer_list<IOutputNode*>&& parameters) {
      return NoSep({Text(function_name), Brackets({Params(std::move(parameters))})});
    }

    ListNode* MemberFuncDef(const String& type, const String& name, std::initializer_list<IOutputNode*>&& parameters,
                            const String& modifiers) {
      return Spaces({Text(type), NoSep({
                                     Text(name),
                                     Brackets({Params(std::move(parameters))}),
                                     Semicolon(),
                                     NewLine(),
                                 })});
    }

    ListNode* ConstRef(const String& type) { return Spaces({Text("const"), NoSep({Text(type), Text("&")})}); }

    ListNode* MemberFuncImpl(const String& return_type, const String& class_name, const String& name,
                             std::initializer_list<IOutputNode*>&& params, const String& modifiers,
                             std::initializer_list<IOutputNode*>&& lines) {
      return FuncImpl(return_type, class_name + "::" + name, std::move(params), modifiers, std::move(lines));
    }

    SnippetNode* Param(const String& name) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(name);
    }

    ListNode* Namespace(const String& name, IOutputNode* content) {
      return Spaces({Text("namespace"), Text(name), Braces({NewLine(), Indent(4, {content})})});
    }

    ListNode* Enum(const String& name, IOutputNode* content) {
      return Spaces({Text("enum"), Text(name),
                     Braces({
                         NewLine(),
                         Indent(2, {content}),
                     }),
                     Semicolon()});
    }

    ListNode* MacroDefine(const String& name, IOutputNode* content) { return Lines({}); }
    ListNode* MacroFunctionDefine(const String& name, std::initializer_list<IOutputNode*> params,
                                  std::initializer_list<IOutputNode*> lines) {
      return Spaces({
          Text("#define"),
          NoSep({
              Text(name),
              Brackets({Params(std::move(params))}),
          }),
          Indent(2, {EscapedLines(std::move(lines))}),
      });
    }

    ListNode* Class(const String& name, IOutputNode* content) {
      return Spaces({Text("class"), Text(name),
                     Braces({
                         NewLine(),
                         Indent(2, {content}),
                     }),
                     Semicolon()});
    }

    ListNode* Class(const String& name, String base, IOutputNode* content) {
      return Spaces({Text("class"), Text(name), Text(": public"), Text(base),
                     Braces({
                         NewLine(),
                         Indent(2, {content}),
                     }),
                     Semicolon()});
    }

  }  // namespace Writer
}  // namespace GodotObjectCompiler
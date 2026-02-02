
#include "output.h"

#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {
  namespace Writer {

    void IndentNode::get_output(IStringWriter* writer) {
      StreamWriter child_writer = StreamWriter();

      for (Ref<Node> child : get_children()) {
        Ref<IOutputNode> output = child->as<IOutputNode>();
        if (output) {
          output->get_output(&child_writer);
        }
      }

      Size total = total_amount();
      String child_result = child_writer.get_string();
      Vector<String> lines = string_split(child_result, "\n");
      for (const String& line : lines) {
        for (Size i = 0; i < total; ++i) {
          writer->write(" ");
        }
        if (!line.empty()) {
          writer->write(line);
        }
        writer->write("\n");
      }
    }

    bool IndentNode::copy_to(Ref<Node> other) const {
      COPY_GUARD(IndentNode, Context)

      target->amount = amount;
      return true;
    }

    Size IndentNode::_total_amount_lazy_get() {
      Ref<IndentNode> parent = find_ancestor<IndentNode>();

      if (parent) {
        return parent->total_amount() + amount;
      }

      return amount;
    }

    void EnclosingNode::get_output(IStringWriter* writer) {
      writer->write(before);
      for (Ref<Node> child : get_children()) {
        Ref<IOutputNode> child_output = child->as<IOutputNode>();
        if (child_output) {
          child_output->get_output(writer);
        }
      }
      writer->write(after);
    }

    bool EnclosingNode::copy_to(Ref<Node> other) const {
      COPY_GUARD(EnclosingNode, Context)

      target->before = before;
      target->after = after;
      return true;
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

        Ref<Node> child = get_child(i);
        Ref<IOutputNode> output = child->as<IOutputNode>();

        if (output) {
          StreamWriter child_writer;
          output->get_output(&child_writer);
          last_empty = child_writer.current_length() == 0 || child_writer.get_string() == delimiter;
          writer->write(child_writer.get_string());
        }
      }

      if (after_last) {
        writer->write(delimiter);
      }

      Brackets({});
    }

    bool ListNode::copy_to(Ref<Node> other) const {
      COPY_GUARD(ListNode, Context)

      target->before_first = before_first;
      target->after_last = after_last;
      target->delimiter = delimiter;
      return true;
    }

    void ReplaceNode::get_output(IStringWriter* writer) {
      for (Ref<Node> child : *this) {
        Ref<IOutputNode> output_node = child->as<IOutputNode>();
        if (output_node) {
          StreamWriter child_writer;
          output_node->get_output(&child_writer);
          writer->write(string_replace(child_writer.get_string(), search, replace));
        }
      }
    }

    bool ReplaceNode::copy_to(Ref<Node> other) const {
      COPY_GUARD(ReplaceNode, Context)

      target->search = search;
      target->replace = replace;
      return true;
    }

    void SnippetNode::get_output(IStringWriter* writer) { writer->write(content); }

    bool SnippetNode::copy_to(Ref<Node> other) const {
      COPY_GUARD(SnippetNode, Node)

      target->content = content;
      return true;
    }

#define ADD_CHILDREN_AND_RET(creator)                                 \
  auto result = ExecutionContext::instance()->get_node_db()->creator; \
  for (Ref<IOutputNode> child : children) {                           \
    Ref<Node> node = std::dynamic_pointer_cast<Node>(child);          \
    if (node) {                                                       \
      result->add_child(node);                                        \
    }                                                                 \
  }                                                                   \
  return result

    Ref<IndentNode> Indent(Size indent, std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<IndentNode>(indent));
    }

    Ref<EnclosingNode> Brackets(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("(", ")"));
    }

    Ref<EnclosingNode> SquareBrackets(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("[", "]"));
    }

    Ref<EnclosingNode> Braces(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("{", "}"));
    }

    Ref<EnclosingNode> Chevrons(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<EnclosingNode>("<", ">"));
    }

    Ref<ListNode> Lines(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>("\n", false, false););
    }

    Ref<ReplaceNode> EscapedLines(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<ReplaceNode>("\n", "\\\n"););
    }

    Ref<ListNode> Spaces(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>(" ", false, false););
    }

    Ref<ListNode> NoSep(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>("", false, false););
    }

    Ref<ListNode> Params(std::initializer_list<Ref<IOutputNode>>&& children) {
      ADD_CHILDREN_AND_RET(create<ListNode>(", ", false, false););
    }

    Ref<ListNode> Param(const String& type, const String& name, Ref<IOutputNode> default_val) {
      return default_val ? Spaces({Text(type), Text(name), Text("="), default_val}) : Spaces({Text(type), Text(name)});
    }

    Ref<ListNode> ConstRefParam(const String& type, const String& name, Ref<IOutputNode> default_val) {
      return default_val ? Spaces({ConstRef(type), Text(name), Text("="), default_val})
                         : Spaces({ConstRef(type), Text(name)});
    }

    Ref<ListNode> LineOfCode(std::initializer_list<Ref<IOutputNode>>&& children) {
      return NoSep({NoSep({std::move(children)}), Semicolon()});
    }

    Ref<SnippetNode> Text(const String& content) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(content);
    }

    Ref<SnippetNode> BoldText(const String& content) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(format("<b>%s</b>", content.c_str()));
    }

    Ref<SnippetNode> StringLiteral(const String& content) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>("\"" + content + "\"");
    }

    Ref<SnippetNode> Semicolon() { return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(";"); }

    Ref<SnippetNode> NewLine() { return ExecutionContext::instance()->get_node_db()->create<SnippetNode>("\n"); }

    Ref<ListNode> ParamDecl(const String& type, const String& name) {
      return Spaces({
          Text(type),
          Text(name),
      });
    }

    Ref<ListNode> FuncDef(const String& modifiers_front, const String& return_type, const String& function_name,
        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers) {
      return Spaces({Text(return_type),
          NoSep({
              Text(function_name),
              Brackets({Params(std::move(params))}),
          }),
          Text(modifiers), Semicolon(), NewLine()});
    }

    Ref<ListNode> FuncImpl(const String& modifiers_front, const String& return_type, const String& function_name,
        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers,
        std::initializer_list<Ref<IOutputNode>>&& lines) {
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

    Ref<ListNode> DeclAssign(const String& type, const String& name, Ref<IOutputNode> value) {
      return LineOfCode({Spaces({Text(type), Text(name), Text("="), value})});
    }

    Ref<ListNode> Assign(const String& variable_name, Ref<IOutputNode> value) {
      return NoSep({Spaces({Text(variable_name), Text("="), value}), Semicolon()});
    }

    Ref<ListNode> Return(const String& name) { return LineOfCode({Spaces({Text("return"), Text(name)})}); }

    Ref<ListNode> FuncCall(const String& function_name, std::initializer_list<Ref<IOutputNode>>&& parameters) {
      return NoSep({Text(function_name), Brackets({Params(std::move(parameters))})});
    }

    Ref<ListNode> MemberFuncDef(const String& type, const String& name,
        std::initializer_list<Ref<IOutputNode>>&& parameters, const String& modifiers) {
      return Spaces({Text(type), NoSep({
                                     Text(name),
                                     Brackets({Params(std::move(parameters))}),
                                     Semicolon(),
                                     NewLine(),
                                 })});
    }

    Ref<ListNode> ConstRef(const String& type) { return Spaces({Text("const"), NoSep({Text(type), Text("&")})}); }

    Ref<ListNode> MemberFuncImpl(const String& return_type, const String& class_name, const String& name,
        std::initializer_list<Ref<IOutputNode>>&& params, const String& modifiers,
        std::initializer_list<Ref<IOutputNode>>&& lines) {
      return FuncImpl("", return_type, class_name + "::" + name, std::move(params), modifiers, std::move(lines));
    }

    Ref<SnippetNode> Param(const String& name) {
      return ExecutionContext::instance()->get_node_db()->create<SnippetNode>(name);
    }

    Ref<SnippetNode> Include(const String& path) { return node_new<SnippetNode>("#include \"" + path + "\""); }

    Ref<ListNode> Namespace(const String& name, Ref<IOutputNode> content) {
      return Spaces({Text("namespace"), Text(name), Braces({NewLine(), Indent(4, {content})})});
    }

    Ref<ListNode> Enum(const String& name, Ref<IOutputNode> content) {
      return Spaces({Text("enum"), Text(name),
          Braces({
              NewLine(),
              Indent(2, {content}),
          }),
          Semicolon()});
    }

    Ref<ListNode> MacroDefine(const String& name, Ref<IOutputNode> content) { return Lines({}); }

    Ref<ListNode> MacroFunctionDefine(const String& name, std::initializer_list<Ref<IOutputNode>> params,
        std::initializer_list<Ref<IOutputNode>> lines) {
      return Spaces({
          Text("#define"),
          NoSep({
              Text(name),
              Brackets({Params(std::move(params))}),
          }),
          Indent(2, {EscapedLines(std::move(lines))}),
      });
    }

    Ref<EnclosingNode> DocComment(Ref<Node> content) {
      return build<EnclosingNode>("/**\n*", "/")
          .with_child(build<ReplaceNode>("\n", "\n*").with_child(build<IndentNode>(2).with_child(content)));
      return build<EnclosingNode>("/**\n*", "\n*/").with_child(build<ReplaceNode>("\n", "\n*").with_child(content));
    }

    Ref<ListNode> Define(const String& name, std::initializer_list<Ref<IOutputNode>> params, const String& content) {
      return Spaces({Text("#define"), NoSep({Text(name), Brackets({Params(std::move(params))})}),
          EscapedLines({Text(content)}), NewLine(), NewLine()});
    }

    Ref<ListNode> Define(const String& name, std::initializer_list<Ref<IOutputNode>> params,
        std::initializer_list<Ref<IOutputNode>>&& lines) {
      return Spaces({Text("#define"), NoSep({Text(name), Brackets({Params(std::move(params))})}),
          EscapedLines({std::move(lines)}), NewLine(), NewLine()});
    }

    Ref<SnippetNode> PragmaOnce() { return node_new<SnippetNode>("#pragma once\n\n"); }

    Ref<ListNode> Class(const String& name, Ref<IOutputNode> content) {
      return Spaces({Text("class"), Text(name),
          Braces({
              NewLine(),
              Indent(2, {content}),
          }),
          Semicolon()});
    }

    Ref<ListNode> Class(const String& name, String base, Ref<IOutputNode> content) {
      return Spaces({Text("class"), Text(name), Text(": public"), Text(base),
          Braces({
              NewLine(),
              Indent(2, {content}),
          }),
          Semicolon()});
    }

  }  // namespace Writer
}  // namespace GodotObjectCompiler
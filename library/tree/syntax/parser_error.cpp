
#include "parser_error.h"

#include "context.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  Error::~Error() {
    if (!handled && ExecutionContext::instance()->get_error_level() >= error_level) {
      print_err(message);
    }
  }

  String Error::to_string() const { return get_type_static(); };

  bool Error::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(ParserError, Node);
    target->message = message;
    return true;
  }

  void Error::write_to(IStructuredWriter* p_writer) {
    Node::write_to(p_writer);
    p_writer->write("error_offending", message);
  }

  void Error::read_from(IStructuredReader* p_reader) {
    Node::read_from(p_reader);
    p_reader->read<String, String>("error_offending");
  }

  void Error::set_handled() { handled = true; }

  GeneratorError::GeneratorError(
      ErrorLevel level, const String& generator_name, const String& user_message, Ref<Node> node) {
    error_level = level;
    StreamWriter writer;
    writer.write(error_level_to_string(level));
    writer.write(" ");
    writer.write(generator_name);
    writer.write(": ");
    writer.write(user_message);
    writer.write("\n");

    if (ExecutionContext::instance()->get_error_detail() == ErrorDetail::FULL) {
      writer.write("\nOccurred while processing node:\n");

      if (Ref<Context> parent = node->get_parent()) {
        Size line = 0;
        String pretty = parent->print_pretty_and_get_child_line(node, line);

        Ref<Context> context = node->as<Context>();
        Size node_lines = context ? context->get_descendant_count() : 1;

        writer.write(extract_lines(pretty, line - std::min(line, static_cast<Size>(3)), line + node_lines + 3, line));
      } else {
        writer.write(extract_lines(node->pretty_print(), 0, 6, 1));
      }
    }

    writer.write("\n");
    message = writer.get_string();
  }

  ParserError::ParserError(ErrorLevel level, const Ref<TreeSitterNode>& node, const String& message)
      : ParserError(level, "TreeSitterParser", message, node->context->file_path, node->context->buffer,
            node->start_point.row + 1, node->start_point.column + 1) {}

  ParserError::ParserError(ErrorLevel level, const String& parser_name, const String& user_message,
      const String& file_path, const String& file_content, Size line, Size column) {
    error_level = level;
    StreamWriter writer;
    writer.write(error_level_to_string(level));
    writer.write(" ");
    writer.write(parser_name);
    writer.write(": ");
    writer.write(file_path);
    writer.write(":");
    writer.write_generic(line);
    writer.write(":");
    writer.write_generic(column);
    writer.write(" ");
    writer.write("\n");
    writer.write(user_message);
    writer.write("\n");

    if (ExecutionContext::instance()->get_error_detail() == ErrorDetail::FULL) {
      writer.write("\nOccurred while processing source:\n");
      writer.write(extract_lines(file_content, line - std::min(line, static_cast<Size>(3)), line + 3, line));
    } else {
      writer.write(extract_lines(file_content, line, line, line));
    }

    message = writer.get_string();
  }

}  // namespace GodotObjectCompiler
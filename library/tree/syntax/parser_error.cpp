
#include "parser_error.h"

#include "library/core/helpers.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  Error::~Error() {
    if (ExecutionContext::instance()->get_error_level() >= error_level) {
      print_err(message);
    }
  }

  String Error::to_string() const { return get_type_static() + ": " + message; };

  bool Error::copy_to(Ref<Node> other) const {
    COPY_GUARD(ParserError, Node);
    target->message = message;
    return true;
  }

  void Error::write_to(IStructuredWriter* writer) {
    Node::write_to(writer);
    writer->write("error_offending", message);
  }

  void Error::read_from(IStructuredReader* reader) {
    Node::read_from(reader);
    reader->read<String, String>("error_offending");
  }

  GeneratorError::GeneratorError(
      ErrorLevel level, const String& generator_name, const String& user_message, Ref<Node> node) {
    StreamWriter writer;
    writer.write(error_level_to_string(level));
    writer.write(" ");
    writer.write(generator_name);
    writer.write(": ");
    writer.write(user_message);

    if (ExecutionContext::instance()->get_error_detail() == ErrorDetail::FULL) {
      writer.write("\nOccurred while processing node:\n");
      writer.write(node->pretty_print());
    }

    writer.write("\n");
    message = writer.get_string();
  }

  ParserError::ParserError(ErrorLevel level, const String& parser_name, const String& user_message,
      const String& file_path, const String& file_content, Size line, Size column) {
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
    writer.write(user_message);

    if (ExecutionContext::instance()->get_error_detail() == ErrorDetail::FULL) {
      writer.write("\nOccurred while processing source:\n");
      writer.write(extract_lines(file_content, line - std::min(line, static_cast<Size>(3)), line + 3, line));
    } else {
      writer.write(extract_lines(file_content, line,line, line));
    }

    message = writer.get_string();
  }

}  // namespace GodotObjectCompiler
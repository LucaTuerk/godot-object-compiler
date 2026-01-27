
#include "parser_error.h"

namespace GodotObjectCompiler {

  String ParserError::to_string() const { return "!! ERROR !!: \n" + offending; };

  bool ParserError::copy_to(Ref<Node> other) const {
    COPY_GUARD(ParserError, Node);
    target->offending = offending;
    return true;
  }

  void ParserError::write_to(IStructuredWriter* writer) {
    Node::write_to(writer);
    writer->write("error_offending", offending);
  }

  void ParserError::read_from(IStructuredReader* reader) {
    Node::read_from(reader);
    reader->read<String, String>("error_offending");
  }

}  // namespace GodotObjectCompiler
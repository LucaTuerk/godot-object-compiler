//
// Created by luca on 20.01.26.
//

#include "literal.h"

namespace GodotObjectCompiler {

  Ref<Literal> Literal::StringLiteral(const String& content) { return node_new<Literal>("\"" + content + "\""); }

  String Literal::to_string() const { return "Literal: " + content; }

  bool Literal::copy_to(Ref<Node> other) const {
    COPY_GUARD(Literal, Node);
    target->content = content;
    return true;
  }

  void Literal::write_to(IStructuredWriter* writer) {
    Node::write_to(writer);
    writer->write("literal_content", content);
  }

  void Literal::read_from(IStructuredReader* reader) {
    Node::read_from(reader);
    content = reader->read<String, String>("literal_content");
  }

}  // namespace GodotObjectCompiler
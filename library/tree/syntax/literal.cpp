//
// Created by luca on 20.01.26.
//

#include "literal.h"

namespace GodotObjectCompiler {

  Ref<Literal> Literal::StringLiteral(const String& content) { return node_new<Literal>("\"" + content + "\""); }

  String Literal::to_string() const { return "Literal: " + content; }

  bool Literal::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(Literal, Node);
    target->content = content;
    return true;
  }

  void Literal::write_to(IStructuredWriter* p_writer) {
    Node::write_to(p_writer);
    p_writer->write("literal_content", content);
  }

  void Literal::read_from(IStructuredReader* p_reader) {
    Node::read_from(p_reader);
    content = p_reader->read<String, String>("literal_content");
  }

}  // namespace GodotObjectCompiler
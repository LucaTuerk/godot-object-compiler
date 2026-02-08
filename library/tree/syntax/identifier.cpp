#include "identifier.h"

namespace GodotObjectCompiler {

  bool Identifier::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(Identifier, Node);
    target->name = name;
    return true;
  }

  String Identifier::to_string() const { return "Identifier: " + name; }

  void Identifier::write_to(IStructuredWriter* p_writer) {
    Node::write_to(p_writer);
    p_writer->write("name", name);
  }

  void Identifier::read_from(IStructuredReader* p_reader) {
    Node::read_from(p_reader);
    name = p_reader->read<String, String>("name");
  }

}  // namespace GodotObjectCompiler

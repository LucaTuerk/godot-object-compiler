#include "identifier.h"

namespace GodotObjectCompiler {

  bool Identifier::copy_to(Ref<Node> other) const {
    COPY_GUARD(Identifier, Node);
    target->name = name;
    return true;
  }

  String Identifier::to_string() const { return "Identifier: " + name; }

  void Identifier::write_to(IStructuredWriter* writer) {
    Node::write_to(writer);
    writer->write("name", name);
  }

  void Identifier::read_from(IStructuredReader* reader) {
    Node::read_from(reader);
    name = reader->read<String, String>("name");
  }

}  // namespace GodotObjectCompiler

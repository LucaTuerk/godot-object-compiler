#include "library/tree/identifier.h"

namespace GodotObjectCompiler {

  bool Identifier::copy_to(Node* other) const {
    COPY_GUARD(Identifier, Node);
    target->name = name;
    return true;
  }

  String Identifier::to_string() const { return "Identifier: " + name; }

  void Identifier::write_to(IWriter* writer) {
    Node::write_to(writer);
    writer->write("name", name);
  }

}  // namespace GodotObjectCompiler

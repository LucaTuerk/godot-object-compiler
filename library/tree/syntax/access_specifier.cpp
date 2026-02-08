#include "access_specifier.h"

namespace GodotObjectCompiler {

  String AccessSpecifier::to_string() const {
    String value = "";

    switch (type) {
      case PUBLIC:
        value += "public";
        break;
      case PRIVATE:
        value += "private";
        break;
      case PROTECTED:
        value += "protected";
        break;
    }

    return "AccessSpecifier: " + value;
  }

  bool AccessSpecifier::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(AccessSpecifier, Node);
    target->type = type;
    return true;
  }

  void AccessSpecifier::write_to(IStructuredWriter* p_writer) {
    Node::write_to(p_writer);
    p_writer->write("type", type);
  }

}  // namespace GodotObjectCompiler
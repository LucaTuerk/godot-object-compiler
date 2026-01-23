#include "type.h"

#include "identifier.h"
#include "library/core/string_writer.h"
#include "modifiers.h"

namespace GodotObjectCompiler {

  String Type::_type_name_lazy_get() {
    StreamWriter writer;
    for (Node* child : get_children()) {
      if (Identifier* id = dynamic_cast<Identifier*>(child)) {
        writer.write(id->name);
      } else if (Reference* ref = dynamic_cast<Reference*>(child)) {
        writer.write("&");
      } else if (Pointer* ptr = dynamic_cast<Pointer*>(child)) {
        writer.write("*");
      } else if (Const* c = dynamic_cast<Const*>(child)) {
        writer.write("const");
      } else if (Static* s = dynamic_cast<Static*>(child)) {
        writer.write("static");
      } else if (Volatile* v = dynamic_cast<Volatile*>(child)) {
        writer.write("volatile");
      }
    }
    return writer.get_string();
  }

}  // namespace GodotObjectCompiler
#include "type.h"

#include "identifier.h"
#include "library/core/string_writer.h"
#include "modifiers.h"

namespace GodotObjectCompiler {

  String Type::_type_name_lazy_get() {
    StreamWriter writer;
    for (Ref<Node> child : get_children()) {
      if (Ref<Identifier> id = child->as<Identifier>()) {
        writer.write(id->name);
      } else if (Ref<Reference> ref = child->as<Reference>()) {
        writer.write("&");
      } else if (Ref<Pointer> ptr = child->as<Pointer>()) {
        writer.write("*");
      } else if (Ref<Const> c = child->as<Const>()) {
        writer.write("const");
      } else if (Ref<Static> s = child->as<Static>()) {
        writer.write("static");
      } else if (Ref<Volatile> v = child->as<Volatile>()) {
        writer.write("volatile");
      }
    }
    return writer.get_string();
  }

}  // namespace GodotObjectCompiler
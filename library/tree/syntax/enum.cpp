#include "enum.h"

namespace GodotObjectCompiler {

  Vector<String> Enum::_value_names_lazy_get() {
    Vector<String> result;

    EnumValues* values = find_child<EnumValues>();

    if (values) {
      for (Node* child : values->get_children()) {
        if (const EnumValue* value = child->as<EnumValue>()) {
          auto name = value->name();
          if (!name.empty()) {
            result.push_back(name);
          }
        }
      }
    }

    return result;
  }

}  // namespace GodotObjectCompiler
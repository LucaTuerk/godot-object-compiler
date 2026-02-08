#include "enum.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Vector<Ref<EnumValue>> Enum::_values_lazy_get() { return find_children<EnumValue>(true); }

  Vector<String> Enum::_value_names_lazy_get() {
    Vector<String> result;

    for (Ref<Node> child : values()) {
      if (const Ref<EnumValue> value = child->as<EnumValue>()) {
        if (auto name = value->name(); !name.empty()) {
          result.push_back(name);
        }
      }
    }

    return result;
  }

  Ref<Literal> EnumValue::_literal_lazy_get() {
    Ref<Literal> value = find_child<Literal>();
    PANIC_COND(!value, "No enum value literal found. This is invalid.")
    return value;
  }

}  // namespace GodotObjectCompiler
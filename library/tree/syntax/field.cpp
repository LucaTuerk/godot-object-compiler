#include "field.h"

#include "modifiers.h"

namespace GodotObjectCompiler {

  Ref<Type> Field::_type_lazy_get() { return find_descendant<Type>(); }

  bool Field::_is_const_lazy_get() { return find_descendant<Const>() != nullptr; }

  bool Field::_is_mutable_lazy_get() { return find_descendant<Mutable>() != nullptr; }

  bool Field::_is_volatile_lazy_get() { return find_descendant<Volatile>() != nullptr; }

  bool Field::_is_static_lazy_get() { return find_descendant<Static>() != nullptr; }

}  // namespace GodotObjectCompiler
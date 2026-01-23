#include "function.h"

#include "modifiers.h"
#include "type.h"

namespace GodotObjectCompiler {

  Type* Function::_type_lazy_get() { return find_child<Type>(); }

  Parameters* Function::_parameters_lazy_get() { return find_child<Parameters>(); }

  bool Function::_is_virtual_lazy_get() { return find_child<Virtual>() != nullptr; }

  bool Function::_is_override_lazy_get() { return find_child<Override>() != nullptr; }

  bool Function::_is_static_lazy_get() { return find_child<Static>() != nullptr; }

  bool Function::_is_const_lazy_get() { return find_child<Const>() != nullptr; }

  bool Function::copy_to(Node* other) const {
    COPY_GUARD(Function, NamedContext);
    // COPY_LAZY(type);
    // COPY_LAZY(parameters);
    // COPY_LAZY(is_virtual);
    // COPY_LAZY(is_override);
    // COPY_LAZY(is_static);
    // COPY_LAZY(is_const);
    return true;
  }

}  // namespace GodotObjectCompiler
#include "function.h"

#include "access_specifier.h"
#include "class.h"
#include "modifiers.h"
#include "struct.h"
#include "type.h"

namespace GodotObjectCompiler {

  Ref<Type> Function::_type_lazy_get() { return find_child<Type>(); }

  Ref<Parameters> Function::_parameters_lazy_get() { return find_child<Parameters>(); }

  Ref<Arguments> Function::_arguments_lazy_get() { return find_child<Arguments>(); }

  bool Function::copy_to(Ref<Node> other) const {
    COPY_GUARD(Function, NamedContext);
    // COPY_LAZY(type);
    // COPY_LAZY(parameters);
    // COPY_LAZY(is_virtual);
    // COPY_LAZY(is_override);
    // COPY_LAZY(is_static);
    // COPY_LAZY(is_const);
    return true;
  }

  Ref<Type> Parameter::_type_lazy_get() { return find_child<Type>(); }

  Ref<Literal> Parameter::_default_value_lazy_get() { return find_child<Literal>(); }

}  // namespace GodotObjectCompiler
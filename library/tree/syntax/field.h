#pragma once

#include "node.h"
#include "type.h"

namespace GodotObjectCompiler {

  class Field : public NamedContext {
    NODE_TYPE(Field);

    LAZY(Field, Ref<Type>, type);
    LAZY(Field, bool, is_const)
    LAZY(Field, bool, is_mutable);
    LAZY(Field, bool, is_volatile);
    LAZY(Field, bool, is_static);
  };

}  // namespace GodotObjectCompiler

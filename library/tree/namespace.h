#pragma once

#include "library/core/lazy.h"
#include "node.h"

namespace GodotObjectCompiler {
  class Field;
  class Function;

  class Namespace : public NamedContext {
    NODE_TYPE(Namespace);

    /* collections */
    LAZY(Namespace, Vector<Namespace*>, namespaces);
    LAZY(Namespace, Vector<Field*>, fields);
    LAZY(Namespace, Vector<Function*>, functions);
  };
}  // namespace GodotObjectCompiler

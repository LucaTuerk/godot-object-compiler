#pragma once

#include "context.h"
#include "library/core/lazy.h"

namespace GodotObjectCompiler {
  class Field;
  class Function;
  class Class;

  class Namespace : public NamedContext {
    NODE_TYPE(Namespace);

    /* collections */
    LAZY(Namespace, Vector<Namespace*>, namespaces);
    LAZY(Namespace, Vector<Namespace*>, child_namespaces);
    LAZY(Namespace, Vector<Class*>, classes)
    LAZY(Namespace, Vector<Class*>, classes_recursive);
    LAZY(Namespace, Vector<Field*>, fields);
    LAZY(Namespace, Vector<Function*>, functions);
  };
}  // namespace GodotObjectCompiler

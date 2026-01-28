#pragma once

#include "context.h"
#include "library/core/lazy.h"

namespace GodotObjectCompiler {

  class Field;
  class Function;
  class Class;
  class Enum;
  class Body;

  class Namespace : public NamedContext {
    NODE_TYPE(Namespace);

    /* collections */
    LAZY(Namespace, Vector<Ref<Namespace>>, namespaces);
    LAZY(Namespace, Vector<Ref<Namespace>>, child_namespaces);
    LAZY(Namespace, Vector<Ref<Class>>, classes)
    LAZY(Namespace, Vector<Ref<Class>>, classes_recursive);
    LAZY(Namespace, Vector<Ref<Enum>>, enums);
    LAZY(Namespace, Vector<Ref<Enum>>, enums_recursive);
    LAZY(Namespace, Vector<Ref<Field>>, fields);
    LAZY(Namespace, Vector<Ref<Function>>, functions);
  };

}  // namespace GodotObjectCompiler

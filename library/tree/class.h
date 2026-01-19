#pragma once

#include <functional>

#include "library/core/lazy.h"
#include "namespace.h"

namespace GodotObjectCompiler {
  class Field;
  class Function;

  class Class : public Namespace {
    NODE_TYPE(Class);

    /* collections */
    LAZY(Class, Vector<Class*>, base_classes);

   public:
    bool copy_to(Node* other) const override;
  };

  class Body : public Context {
    NODE_TYPE(Body);
  };

}  // namespace GodotObjectCompiler

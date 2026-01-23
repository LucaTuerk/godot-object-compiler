#pragma once

#include <functional>

#include "library/core/lazy.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  class Field;
  class Function;
  class Attribute;
  class Body;

  class Class : public Namespace {
    NODE_TYPE(Class);

    LAZY(Class, Body*, class_body);

    /* collections */
    LAZY(Class, Vector<Class*>, base_classes);
    LAZY(Class, bool, has_generated_class_attribute);
    LAZY(Class, Vector<Attribute*>, attributes);

   public:

    bool copy_to(Node* other) const override;
  };

}  // namespace GodotObjectCompiler

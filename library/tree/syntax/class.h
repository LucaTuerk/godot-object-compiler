#pragma once

#include "attribute.h"
#include "library/core/lazy.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  class Field;
  class Function;
  class Body;

  class Class : public Namespace {
    NODE_TYPE(Class);

    /* collections */
    LAZY(Class, Vector<String>, direct_bases_names);
    LAZY(Class, Vector<Ref<Class>>, base_classes);
    LAZY(Class, bool, has_generated_class_attribute);
    LAZY(Class, Vector<Ref<Attribute>>, attributes);

   public:

    bool copy_to(Ref<Node> other) const override;
  };

}  // namespace GodotObjectCompiler

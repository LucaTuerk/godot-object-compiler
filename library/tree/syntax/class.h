#pragma once

#include "attribute.h"
#include "library/core/lazy.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  class Field;
  class Function;
  class Body;

  class BaseClasses : public Context {
    NODE_TYPE(BaseClasses)
  };

  class Class : public Namespace {
    NODE_TYPE(Class);

    /* collections */
    LAZY(Class, Vector<String>, direct_bases_names);
    LAZY(Class, Vector<Ref<Class>>, base_classes);
    LAZY(Class, bool, has_generated_class_attribute);
    LAZY(Class, Vector<Ref<Attribute>>, attributes);
    // Functions
    LAZY(Class, Vector<Ref<Function>>, member_functions);
    LAZY(Class, Vector<Ref<Function>>, public_member_functions);
    LAZY(Class, Vector<Ref<Function>>, protected_member_functions);
    LAZY(Class, Vector<Ref<Function>>, private_member_functions);
    // Fields
    LAZY(Class, Vector<Ref<Field>>, member_fields);
    LAZY(Class, Vector<Ref<Field>>, public_member_fields);
    LAZY(Class, Vector<Ref<Field>>, protected_member_fields);
    LAZY(Class, Vector<Ref<Field>>, private_member_fields);

   public:

    bool copy_to(Ref<Node> p_other) const override;
  };

}  // namespace GodotObjectCompiler

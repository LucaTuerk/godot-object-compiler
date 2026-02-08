#pragma once
#include "context.h"
#include "literal.h"

namespace GodotObjectCompiler {

  class EnumValue;

  class Enum : public NamedContext {
    NODE_TYPE(Enum);

    LAZY(Enum, Vector<Ref<EnumValue>>, values);
    LAZY(Enum, Vector<String>, value_names);
  };

  class EnumValues : public Context {
    NODE_TYPE(EnumValues);
  };

  class EnumValue : public NamedContext {
    NODE_TYPE(EnumValue);

    LAZY(EnumValue, Ref<Literal>, literal);
  };

}  // namespace GodotObjectCompiler

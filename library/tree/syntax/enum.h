#pragma once
#include "context.h"

namespace GodotObjectCompiler {

  class Enum : public NamedContext {
    NODE_TYPE(Enum);

    LAZY(Enum, Vector<String>, value_names);
  };

  class EnumValues : public Context {
    NODE_TYPE(EnumValues);
  };

  class EnumValue : public NamedContext {
    NODE_TYPE(EnumValue);
  };
}  // namespace GodotObjectCompiler

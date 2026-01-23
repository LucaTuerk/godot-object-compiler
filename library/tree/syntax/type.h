#pragma once

#include "context.h"

namespace GodotObjectCompiler {
  class Reference : public Node {
    NODE_TYPE(Reference);
  };

  class Pointer : public Node {
    NODE_TYPE(Pointer);
  };

  class Type : public NamedContext {
    NODE_TYPE(Type);

    LAZY(Type, String, type_name);
  };

  class PlaceholderType : public Node {
    NODE_TYPE(PlaceholderType);
  };
}  // namespace GodotObjectCompiler

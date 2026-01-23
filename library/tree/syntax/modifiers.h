#pragma once

#include "node.h"

namespace GodotObjectCompiler {
  class Virtual : public Node {
    NODE_TYPE(Virtual);
  };

  class Static : public Node {
    NODE_TYPE(Static);
  };

  class Inline : public Node {
    NODE_TYPE(Inline);
  };

  class Override : public Node {
    NODE_TYPE(Override);
  };

  class TypeQualifier : public Node {
    NODE_TYPE(TypeQualifier);
  };

  class Const : public TypeQualifier {
    NODE_TYPE(Const);
  };

  class Mutable : public TypeQualifier {
    NODE_TYPE(Mutable);
  };

  class Volatile : public TypeQualifier {
    NODE_TYPE(Volatile);
  };
}  // namespace GodotObjectCompiler

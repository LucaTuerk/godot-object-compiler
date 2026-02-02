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

  class ConstExpression : public TypeQualifier {
    NODE_TYPE(ConstExpression);
  };

  class Unsigned : public TypeQualifier {
    NODE_TYPE(Unsigned);
  };

  class Signed : public TypeQualifier {
    NODE_TYPE(Signed);
  };

  class Short : public TypeQualifier {
    NODE_TYPE(Short)
  };

  class Long : public TypeQualifier {
    NODE_TYPE(Long)
  };

}  // namespace GodotObjectCompiler

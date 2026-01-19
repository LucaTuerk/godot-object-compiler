#pragma once

#include "node.h"

namespace GodotObjectCompiler {
  class Type;

  class Parameters : public Context {
    NODE_TYPE(Parameters);
  };

  class TemplateParameters : public Context {
    NODE_TYPE(TemplateParameters);
  };

  class Parameter : public NamedContext {
    NODE_TYPE(Parameter);
  };

  class Function : public NamedContext {
    NODE_TYPE(Function);

    /* child nodes */
    LAZY(Function, Type*, type);
    LAZY(Function, Parameters*, parameters);

    /* info */
    LAZY(Function, bool, is_virtual);
    LAZY(Function, bool, is_override);
    LAZY(Function, bool, is_static);
    LAZY(Function, bool, is_const);

   public:
    bool copy_to(Node* other) const override;
  };

}  // namespace GodotObjectCompiler

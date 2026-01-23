#pragma once

#include "context.h"
#include "library/core/lazy.h"
#include "literal.h"

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

    LAZY(Parameter, Type*, type)
    LAZY(Parameter, Literal*, default_value);
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

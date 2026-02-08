#pragma once

#include "access_specifier.h"
#include "context.h"
#include "library/core/lazy.h"
#include "literal.h"
#include "member.h"

namespace GodotObjectCompiler {

  class Type;

  class Arguments : public Context {
    NODE_TYPE(Arguments)
  };

  class Argument : public Context {
    NODE_TYPE(Argument)
  };

  class Parameters : public Context {
    NODE_TYPE(Parameters);
  };

  class TemplateParameters : public Context {
    NODE_TYPE(TemplateParameters);
  };

  class TemplateArguments : public Context {
    NODE_TYPE(TemplateArguments);
  };

  class Parameter : public NamedContext {
    NODE_TYPE(Parameter);

    LAZY(Parameter, Ref<Type>, type)
    LAZY(Parameter, Ref<Literal>, default_value);
  };

  class Function : public Member {
    NODE_TYPE(Function);

    /* child nodes */
    LAZY(Function, Ref<Type>, type);
    LAZY(Function, Ref<Parameters>, parameters);
    LAZY(Function, Ref<Arguments>, arguments);

   public:

    bool copy_to(Ref<Node> other) const override;
  };

}  // namespace GodotObjectCompiler

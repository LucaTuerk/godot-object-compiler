#pragma once

#include "context.h"
#include "function.h"

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
    LAZY(Type, String, type_name_untemplated);
    LAZY(Type, String, type_name_unmodified);
    LAZY(Type, bool, is_template_type);
    LAZY(Type, Size, template_argument_count);
    LAZY(Type, Ref<TemplateArguments>, template_arguments);
  };

  class PlaceholderType : public Node {
    NODE_TYPE(PlaceholderType);
  };

}  // namespace GodotObjectCompiler

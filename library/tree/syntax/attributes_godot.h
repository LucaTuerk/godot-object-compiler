#pragma once
#include "attribute.h"
#include "node.h"

namespace GodotObjectCompiler {

  class GodotGeneratedBodyAttribute : public ClassAttribute {
    NODE_TYPE(GodotGeneratedBodyAttribute)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_GENERATED_BODY)
    /* GodotGeneratedBodyAttribute Parameter Options*/
    ATTRIBUTE_REGISTER_PARAMETERS(NoArgs)
  };

  class GodotPropertyAttribute : public FieldAttribute {
    NODE_TYPE(GodotPropertyAttribute)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_PROPERTY)
    /* GodotPropertyAttribute Parameter Options*/
    ATTRIBUTE_REGISTER_PARAMETERS(NoArgs)
    ATTRIBUTE_REGISTER_PARAMETERS(Hint, STRING)
    ATTRIBUTE_REGISTER_PARAMETERS(GetSet, MEMBER_FUNCTION, MEMBER_FUNCTION)
    ATTRIBUTE_REGISTER_PARAMETERS(HintGetSet, STRING, MEMBER_FUNCTION, MEMBER_FUNCTION)
  };

  class GodotFunctionAttribute : public FunctionAttribute {
    NODE_TYPE(GodotFunctionAttribute)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_FUNCTION)
    /* GodotFunctionAttribute Parameter Options*/
    ATTRIBUTE_REGISTER_PARAMETERS(NoArgs)
  };

}  // namespace GodotObjectCompiler

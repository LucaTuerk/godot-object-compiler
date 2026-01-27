#pragma once
#include "library/attribute_db.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/enum.h"
#include "library_godot/attributes/attribute_parameters_godot.h"

namespace GodotObjectCompiler {

  class GodotGeneratedBodyAttribute : public GeneratedBodyAttribute {
    ATTRIBUTE_TYPE(GodotGeneratedBodyAttribute, Class, CONTAINING)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_GENERATED_BODY)
  };

  class GodotGeneratedGlobalAttribute : public GeneratedGlobalAttribute {
    ATTRIBUTE_TYPE(GodotGeneratedGlobalAttribute, Namespace, CONTAINING)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_GENERATED_GLOBAL)
  };

  class GodotClassAttribute : public Attribute {
    ATTRIBUTE_TYPE(GodotClassAttribute, Class, NEXT)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_CLASS)
  };

  class GodotEnumAttribute : public Attribute {
    ATTRIBUTE_TYPE(GodotEnumAttribute, Enum, NEXT)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_ENUM)
  };

  class GodotPropertyAttribute : public FieldAttribute {
    ATTRIBUTE_TYPE(GodotPropertyAttribute, Field, NEXT)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_PROPERTY)
    // ATTRIBUTE_PARAMS(PropertyHintParameters)
    // ATTRIBUTE_PARAMS(PropertyUsageParameters)
  };

  class GodotFunctionAttribute : public FunctionAttribute {
    ATTRIBUTE_TYPE(GodotFunctionAttribute, Function, NEXT)
    ATTRIBUTE_REGISTER_DEFAULT_MACRO(GODOT_FUNCTION)
  };

}  // namespace GodotObjectCompiler

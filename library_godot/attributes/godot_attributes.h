#pragma once
#include "godot_module_init_level.h"
#include "godot_variant_type.h"
#include "godot_property_hint.h"
#include "godot_property_usage_flags.h"
#include "godot_class_type.h"
#include "library/attribute_db.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler {

  class GodotAttributeWithParams : public Attribute {
    ATTRIBUTE_TYPE(GodotAttributeWithParams, Node, NONE)
    Ref<IAttributeArgumentParser> get_argument_parser() override;

    LAZY(GodotAttributeWithParams, Ref<Arguments>, arguments)
  };

  class GodotGeneratedBodyAttribute : public GeneratedBodyAttribute {
    ATTRIBUTE_TYPE(GodotGeneratedBodyAttribute, Class, CONTAINING)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_GENERATED_BODY)
  };

  class GodotGeneratedGlobalAttribute : public GeneratedGlobalAttribute {
    ATTRIBUTE_TYPE(GodotGeneratedGlobalAttribute, Namespace, CONTAINING)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_GENERATED_GLOBAL)
  };

  class GodotClassAttribute : public GodotAttributeWithParams {
    ATTRIBUTE_TYPE(GodotClassAttribute, Class, NEXT)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_CLASS)

    ATTRIBUTE_REGISTER_PARAMETERS(GodotClassType)
    ATTRIBUTE_REGISTER_PARAMETERS(GodotModuleInitializationLevel)
  };

  class GodotEnumAttribute : public GodotAttributeWithParams {
    ATTRIBUTE_TYPE(GodotEnumAttribute, Enum, NEXT)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_ENUM)
  };

  class GodotPropertyAttribute : public GodotAttributeWithParams {
    ATTRIBUTE_TYPE(GodotPropertyAttribute, Field, NEXT)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_PROPERTY)

    ATTRIBUTE_REGISTER_PARAMETERS(GodotVariantType)
    ATTRIBUTE_REGISTER_PARAMETERS(GodotPropertyHint)
    ATTRIBUTE_REGISTER_PARAMETERS(GodotPropertyUsageFlags)
  };

  class GodotFunctionAttribute : public GodotAttributeWithParams {
    ATTRIBUTE_TYPE(GodotFunctionAttribute, Function, NEXT)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_FUNCTION)
  };

  class GodotSignalAttribute : public GodotAttributeWithParams {
    ATTRIBUTE_TYPE(GodotSignalAttribute, Function, NEXT)
    ATTRIBUTE_DEFAULT_MACRO(GODOT_SIGNAL)
  };

}  // namespace GodotObjectCompiler

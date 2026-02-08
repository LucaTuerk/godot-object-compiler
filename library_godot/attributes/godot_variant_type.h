
#pragma once
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  class GodotVariantTypeArgument : public Argument {
    NODE_TYPE(GodotVariantTypeArgument)

    LAZY(GodotVariantTypeArgument, String, godot_variant_type)
  };

  class GodotVariantTypeParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotVariantTypeParameterType)

   public:

    String get_return_type() override;

    Vector<String> get_value_names() override;

    Vector<Argument> get_arguments() override;

    Ref<GodotObjectCompiler::Argument> create_argument() override;

    bool get_variant_type_for_value_name(const String& p_value_name, String& r_variant_type);

   private:

    LAZY(GodotVariantTypeParameterType, Vector<String>, value_names)
    Dictionary<String, String> _value_name_to_godot_variant_type;
  };

}  // namespace GodotObjectCompiler

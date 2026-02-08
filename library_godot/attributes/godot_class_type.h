
#pragma once
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  class GodotClassTypeArgument : public Argument {
    NODE_TYPE(GodotClassTypeArgument);
  };

  class GodotClassTypeParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotClassTypeParameterType);

   public:

    String get_return_type() override;

    Vector<String> get_value_names() override;

    Vector<Argument> get_arguments() override;

    Ref<GodotObjectCompiler::Argument> create_argument() override;

    bool get_macro_for_value_name(const String& p_value_name, String& r_macro);

    private:
    LAZY(GodotClassTypeParameterType, Vector<String>, value_names);
    Dictionary<String,String> _value_name_to_macro;
  };

}  // namespace GodotObjectCompiler

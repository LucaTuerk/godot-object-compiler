
#pragma once
#include "../../library/generator/attribute_parameter_type.h"
#include "library/core/lazy.h"
#include "library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class GodotPropertyHintArgument : public Argument {
    NODE_TYPE(GodotPropertyHintArgument);

    LAZY(GodotPropertyHintArgument, String, godot_property_hint)
    LAZY(GodotPropertyHintArgument, String, hint_string)
  };

  class GodotPropertyHintParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotPropertyHintParameterType);

   public:

    String get_return_type() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    Ref<GodotObjectCompiler::Argument> create_argument() override;

    bool get_godot_hint_for_value_name(const String& name, String& macro);

   private:

    LAZY(GodotPropertyHintParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_hint_types;
  };

}  // namespace GodotObjectCompiler

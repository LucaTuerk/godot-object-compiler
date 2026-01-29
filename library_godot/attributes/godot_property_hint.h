
#pragma once
#include "../../library/generator/attribute_parameter_type.h"
#include "library/core/lazy.h"
#include "library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class GodotPropertyHintArgument : public Argument {
    NODE_TYPE(GodotPropertyHintArgument)
  };

  class GodotPropertyHintParameterType : public IAttributeParameterType {
   public:

    String get_type_name() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    String get_godot_hint_for_value_name(const String& name);
    Ref<GodotObjectCompiler::Argument> create_argument() override;

   private:

    LAZY(GodotPropertyHintParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_hint_types;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "../../library/generator/attribute_parameter_type.h"
#include "../../library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class Class;

  class GodotModuleInitializationLevelArgument : public Argument {
    NODE_TYPE(GodotModuleInitializationLevelArgument);
  };

  class GodotModuleInitializationLevelParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotModuleInitializationLevelParameterType)

   public:

    String get_return_type() override;

    Vector<String> get_value_names() override;

    Vector<Argument> get_arguments() override;

    Ref<GodotObjectCompiler::Argument> create_argument() override;

    bool get_godot_init_level_for_value_name(const String& p_value_name, String& r_godot_init_level);

   private:

    LAZY(GodotModuleInitializationLevelParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_init_levels;
  };

}  // namespace GodotObjectCompiler

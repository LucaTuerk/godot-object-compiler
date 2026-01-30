#pragma once
#include "../../library/generator/attribute_parameter_type.h"
#include "../../library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class GodotModuleInitializationLevelArgument : public Argument {
    NODE_TYPE(GodotModuleInitializationLevelArgument);
  };

  class GodotModuleInitializationLevelParameterType : public IAttributeParameterType {
   public:

    String get_type_name() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    Ref<GodotObjectCompiler::Argument> create_argument() override;

   private:

    LAZY(GodotModuleInitializationLevelParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_init_levels;
  };

}  // namespace GodotObjectCompiler

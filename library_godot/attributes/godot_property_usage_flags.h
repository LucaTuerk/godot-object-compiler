
#pragma once
#include "../../library/generator/attribute_parameter_type.h"
#include "library/core/lazy.h"
#include "library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class GodotPropertyUsageFlagsArgument : public Argument {
    NODE_TYPE(GodotPropertyUsageFlagsArgument);
  };

  class GodotPropertyUsageFlagsParameterType : public IAttributeParameterType {
   public:

    FeatureFlags get_features() override;
    String get_type_name() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    String get_godot_usage_flag_for_value_name(const String& name);
    Ref<GodotObjectCompiler::Argument> create_argument() override;

   private:

    LAZY(GodotPropertyUsageFlagsParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_usage_flags;
  };

}  // namespace GodotObjectCompiler

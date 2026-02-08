
#pragma once
#include "library/core/lazy.h"
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/syntax/node.h"

namespace GodotObjectCompiler {

  class GodotPropertyUsageFlagsArgument : public Argument {
    NODE_TYPE(GodotPropertyUsageFlagsArgument);

    LAZY(GodotPropertyUsageFlagsArgument, String, godot_property_usage_flag)
  };

  class GodotPropertyUsageFlagsParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotPropertyUsageFlagsParameterType);

   public:

    FeatureFlags get_features() override;

    String get_return_type() override;

    Vector<String> get_value_names() override;

    Vector<Argument> get_arguments() override;

    Ref<GodotObjectCompiler::Argument> create_argument() override;

    bool get_godot_usage_flag_for_value_name(const String& p_name, String& r_usage_flag);

   private:

    LAZY(GodotPropertyUsageFlagsParameterType, Vector<String>, value_names);
    Dictionary<String, String> _godot_usage_flags;
  };

}  // namespace GodotObjectCompiler

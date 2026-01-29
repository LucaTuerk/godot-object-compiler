
#include "../../library/type_db.h"
#include "godot_property_usage_flags.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler {

  IAttributeParameterType::FeatureFlags GodotPropertyUsageFlagsParameterType::get_features() {
    return FEATURE_FLAG;
  }

  String GodotPropertyUsageFlagsParameterType::get_type_name() {
    return "GOC_PropertyUsageFlags";
  }

  Vector<String> GodotPropertyUsageFlagsParameterType::get_value_names() {
    return value_names();
  }

  Vector<IAttributeParameterType::Argument> GodotPropertyUsageFlagsParameterType::get_arguments() {
    return {};
  }

  String GodotPropertyUsageFlagsParameterType::get_godot_usage_flag_for_value_name(const String& name) {
    if (auto itr = _godot_usage_flags.find(name); itr != _godot_usage_flags.end()) {
      return itr->second;
    }
    return "";
  }

  Ref<Argument> GodotPropertyUsageFlagsParameterType::create_argument() {
    return node_new<GodotPropertyUsageFlagsArgument>();
  }

  Vector<String> GodotPropertyUsageFlagsParameterType::_value_names_lazy_get() {
    Ref<Enum> prop_use_enum = TypeDB::instance()->get_type_data<Enum>("PropertyUsageFlags");
    if (!prop_use_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : prop_use_enum->value_names()) {
      auto final_name = string_replace(value_name, "PROPERTY_", "");
      _godot_usage_flags[final_name] = value_name;
      return_value.push_back(macro_case_to_pascal_case(final_name));
    }
    return return_value;
  }

}  // namespace GodotObjectCompiler
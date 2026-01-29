
#include "../../library/type_db.h"
#include "godot_property_hint.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler {

  String GodotPropertyHintParameterType::get_type_name() { return "GOC_PropertyHint"; }

  Vector<String> GodotPropertyHintParameterType::get_value_names() {
    return value_names();
  }

  Vector<IAttributeParameterType::Argument> GodotPropertyHintParameterType::get_arguments() {
    return {{ARG_STRING, "hint_string", true}};
  }

  String GodotPropertyHintParameterType::get_godot_hint_for_value_name(const String& name) {
    if (auto itr = _godot_hint_types.find(name); itr != _godot_hint_types.end()) {
      return itr->second;
    }
    return "";
  }

  Ref<Argument> GodotPropertyHintParameterType::create_argument() {
    return node_new<GodotPropertyHintArgument>();
  }

  Vector<String> GodotPropertyHintParameterType::_value_names_lazy_get() {
    Ref<Enum> prop_hint_enum = TypeDB::instance()->get_type_data<Enum>("PropertyHint");
    if (!prop_hint_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : prop_hint_enum->value_names()) {
      return_value.push_back(macro_case_to_pascal_case(string_replace(value_name, "PROPERTY_", "")));
    }
    return return_value;
  }

}  // namespace GodotObjectCompiler

#include "godot_class_type.h"

#include "../assumptions.h"

namespace GodotObjectCompiler {

  String GodotClassTypeParameterType::get_return_type() {
    return "GOC_GodotClassType";
  }

  Vector<String> GodotClassTypeParameterType::get_value_names() {
    return value_names();
  }

  Vector<IAttributeParameterType::Argument> GodotClassTypeParameterType::get_arguments() {
    return {};
  }

  bool GodotClassTypeParameterType::get_macro_for_value_name(const String& value_name, String& macro) {
    _value_names_lazy.poke();

    auto itr = _value_name_to_macro.find(value_name);
    if (itr == _value_name_to_macro.end()) {
      macro = "";
      return false;
    }

    macro = itr->second;
    return true;
  }

  Ref<Argument> GodotClassTypeParameterType::create_argument() {
    return node_new<GodotClassTypeArgument>();
  }

  Vector<String> GodotClassTypeParameterType::_value_names_lazy_get() {
    Vector<String> godot_macros = {
      AssumedGodotTypes::GDREGISTER_CLASS().type_name,
      AssumedGodotTypes::GDREGISTER_VIRTUAL_CLASS().type_name,
      AssumedGodotTypes::GDREGISTER_ABSTRACT_CLASS().type_name,
      AssumedGodotTypes::GDREGISTER_INTERNAL_CLASS().type_name,
      AssumedGodotTypes::GDREGISTER_RUNTIME_CLASS().type_name,
      AssumedGodotTypes::GDREGISTER_NATIVE_STRUCT().type_name,
    };

    Vector<String> value_names = {"RegisterCustom"};
    for (const String& macro : godot_macros) {
      auto value_name = macro_case_to_pascal_case(string_replace(macro, "GDREGISTER_", ""));
      value_name = value_name == "Class" ? "GodotClass" : value_name;
      _value_name_to_macro[value_name] = macro;
      value_names.push_back(value_name);
    }
    return value_names;
  }

}  // namespace GodotObjectCompiler
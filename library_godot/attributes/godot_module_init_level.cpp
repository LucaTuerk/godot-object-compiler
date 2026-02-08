#include "godot_module_init_level.h"

#include "../assumptions.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  String GodotModuleInitializationLevelParameterType::get_return_type() { return "GOC_ModuleInitializationLevel"; }

  Vector<String> GodotModuleInitializationLevelParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotModuleInitializationLevelParameterType::get_arguments() { return {}; }

  Ref<Argument> GodotModuleInitializationLevelParameterType::create_argument() {
    return node_new<GodotModuleInitializationLevelArgument>();
  }

  bool GodotModuleInitializationLevelParameterType::get_godot_init_level_for_value_name(
      const String& p_value_name, String& r_godot_init_level) {
    _value_names_lazy.poke();

    auto itr = _godot_init_levels.find(p_value_name);
    if (itr == _godot_init_levels.end()) {
      r_godot_init_level = "";
      return false;
    }

    r_godot_init_level = itr->second;
    return true;
  }

  Vector<String> GodotModuleInitializationLevelParameterType::_value_names_lazy_get() {
    using namespace AssumedGodotTypes;

    Ref<Enum> init_level_enum = TypeDB::instance()->get_type_data<Enum>(ModuleInitializationLevel().qualified_name);
    if (!init_level_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : init_level_enum->value_names()) {
      String modified = macro_case_to_pascal_case(string_replace(value_name, "MODULE_INITIALIZATION_", ""));
      return_value.push_back(modified);
      _godot_init_levels[modified] = value_name;
    }

    return return_value;
  }

}  // namespace GodotObjectCompiler
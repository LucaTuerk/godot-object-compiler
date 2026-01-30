//
// Created by luca on 30.01.26.
//

#include "godot_module_init_level.h"

#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  String GodotModuleInitializationLevelParameterType::get_type_name() { return "GOC_ModuleInitializationLevel"; }

  Vector<String> GodotModuleInitializationLevelParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotModuleInitializationLevelParameterType::get_arguments() { return {}; }

  Ref<GodotObjectCompiler::Argument> GodotModuleInitializationLevelParameterType::create_argument() {
    return node_new<GodotModuleInitializationLevelArgument>();
  }

  Vector<String> GodotModuleInitializationLevelParameterType::_value_names_lazy_get() {
    Ref<Enum> init_level_enum = TypeDB::instance()->get_type_data<Enum>("ModuleInitializationLevel");
    if (!init_level_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : init_level_enum->value_names()) {
      return_value.push_back(macro_case_to_pascal_case(string_replace(value_name, "MODULE_INITIALIZATION_", "")));
    }

    return return_value;
  }

}  // namespace GodotObjectCompiler
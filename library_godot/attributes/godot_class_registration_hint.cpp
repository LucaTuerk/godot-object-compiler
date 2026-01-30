//
// Created by luca on 30.01.26.
//

#include "godot_class_registration_hint.h"

namespace GodotObjectCompiler {

  String GodotClassRegistrationHintParameterType::get_type_name() {
    return "GOC_ClassRegistrationHint";
  }

  Vector<String> GodotClassRegistrationHintParameterType::get_value_names() {
    return {
      "RegisterGlobal",
      "RegisterInternal",
      "RegisterCustom"
    };
  }

  Vector<IAttributeParameterType::Argument> GodotClassRegistrationHintParameterType::get_arguments() {
    return {};
  }

  Ref<Argument> GodotClassRegistrationHintParameterType::create_argument() {
    return node_new<GodotClassRegistrationHint>();
  }

}  // namespace GodotObjectCompiler
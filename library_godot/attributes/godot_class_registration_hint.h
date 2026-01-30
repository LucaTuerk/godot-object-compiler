#pragma once

#include "../../library/generator/attribute_parameter_type.h"

namespace GodotObjectCompiler {

  class GodotClassRegistrationHint : public Argument {
    NODE_TYPE(GodotClassRegistrationHint)
  };

  class GodotClassRegistrationHintParameterType : public IAttributeParameterType {
   public:

    String get_type_name() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    Ref<GodotObjectCompiler::Argument> create_argument() override;
  };
}  // namespace GodotObjectCompiler


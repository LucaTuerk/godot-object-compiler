
#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotSignalGenerator : public IClassGenerator<GodotSignalAttribute> {
    GENERATOR(GodotSignalGenerator)
   public:

    Ref<GeneratorError> do_generate(Ref<Class> p_target_class, Ref<GodotSignalAttribute> p_attribute,
        Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotSignalGenerator)

}  // namespace GodotObjectCompiler

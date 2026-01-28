
#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotFunctionGenerator : public IClassGenerator<GodotFunctionAttribute> {
    GENERATOR(GodotFunctionGenerator)

   public:

    Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<GodotFunctionAttribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotFunctionGenerator)

}  // namespace GodotObjectCompiler


#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/attributes_godot.h"

namespace GodotObjectCompiler {

  class GodotEnumGenerator : public IClassGenerator<GodotEnumAttribute> {
    GENERATOR(GodotEnumGenerator)
   public:

    Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<GodotEnumAttribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotEnumGenerator)

}  // namespace GodotObjectCompiler

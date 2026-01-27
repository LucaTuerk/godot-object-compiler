
#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/attributes_godot.h"

namespace GodotObjectCompiler {

  class GodotFunctionGenerator : public IClassGenerator<GodotFunctionAttribute> {
    GENERATOR(GodotFunctionGenerator)

   public:

    GeneratorError* do_generate(Class* target_class, GodotFunctionAttribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotFunctionGenerator)

}  // namespace GodotObjectCompiler

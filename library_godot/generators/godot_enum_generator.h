
#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/attributes_godot.h"

namespace GodotObjectCompiler {

  class GodotEnumGenerator : public IClassGenerator<GodotEnumAttribute> {
    GENERATOR(GodotEnumGenerator)
   public:

    GeneratorError* do_generate(Class* target_class, GodotEnumAttribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotEnumGenerator)

}  // namespace GodotObjectCompiler

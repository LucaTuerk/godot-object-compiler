
#pragma once
#include "library/generator/generator.h"
#include "library_godot/attributes/attributes_godot.h"
#include "godot_property_generator.h"

namespace GodotObjectCompiler {

  class GodotClassGenerator : public IClassGenerator<GodotClassAttribute> {
    GENERATOR(GodotClassGenerator);

     GeneratorError* do_generate(Class* target_class, GodotClassAttribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) override;
  };

}  // namespace GodotoObjectCompiler

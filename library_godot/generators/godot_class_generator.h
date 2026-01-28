
#pragma once
#include "godot_property_generator.h"
#include "library/generator/generator.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotClassGenerator : public IClassGenerator<GodotClassAttribute> {
    GENERATOR(GodotClassGenerator);

     Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<GodotClassAttribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) override;
  };

}  // namespace GodotoObjectCompiler

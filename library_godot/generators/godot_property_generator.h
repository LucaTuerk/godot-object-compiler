
#pragma once
#include "library/attribute_db.h"
#include "library/generator/generator.h"
#include "library/tree/output/generator_error.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotPropertyGenerator : public IClassGenerator<GodotPropertyAttribute> {
    GENERATOR(GodotPropertyGenerator);

   public:
   Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> target_class, Ref<GodotPropertyAttribute> attribute, Ref<Context> default_values) override;

    Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<GodotPropertyAttribute> attribute,
        Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotPropertyGenerator);

}  // namespace GodotObjectCompiler

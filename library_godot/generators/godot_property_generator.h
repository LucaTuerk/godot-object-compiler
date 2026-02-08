
#pragma once
#include "library/attribute_db.h"
#include "library/generator/generator.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotPropertyGenerator : public IClassGenerator<GodotPropertyAttribute> {
    GENERATOR(GodotPropertyGenerator);

   public:
   Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute, Ref<Context> p_default_values) override;

    Ref<GeneratorError> do_generate(Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute,
       Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) override;
  };

  REGISTER_CLASS_GENERATOR(GodotPropertyGenerator);

}  // namespace GodotObjectCompiler

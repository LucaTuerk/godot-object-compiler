
#pragma once
#include "godot_initialization_generator.h"
#include "godot_property_generator.h"
#include "library/generator/generator.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotClassGenerator : public IClassGenerator<GodotClassAttribute>, public IGodotInitializationGenerator {
    GENERATOR(GodotClassGenerator);

   public:

    Ref<GeneratorError> generate_initialization(Ref<Class> p_target_class, Ref<GodotClassAttribute> p_class_attribute,
        Ref<Context> p_initialize_content, Ref<Context> p_uninitialize_content) override;

    Ref<GeneratorError> generate_startup(Ref<Class> p_target_class, Ref<GodotClassAttribute> p_class_attribute,
        Ref<Context> p_startup_content, Ref<Context> p_shutdown_content) override;


   private:
    Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute, Ref<Context> p_default_values) override;

    Ref<GeneratorError> do_generate(Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute,
        Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) override;
  };

}  // namespace GodotObjectCompiler

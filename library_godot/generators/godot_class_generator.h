
#pragma once
#include "godot_initialization_generator.h"
#include "godot_property_generator.h"
#include "library/generator/generator.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotClassGenerator : public IClassGenerator<GodotClassAttribute>, public IGodotInitializationGenerator {
    GENERATOR(GodotClassGenerator);

   public:

    Ref<GeneratorError> generate_initialization(Ref<Class> target_class, Ref<GodotClassAttribute> class_attribute,
        Ref<Context> initialize_content, Ref<Context> uninitialize_content) override;

    Ref<GeneratorError> generate_startup(Ref<Class> target_class, Ref<GodotClassAttribute> class_attribute,
        Ref<Context> startup_content, Ref<Context> shutdown_content) override;


   private:
    Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> target_class, Ref<GodotClassAttribute> attribute, Ref<Context> default_values) override;

    Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<GodotClassAttribute> attribute,
        Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) override;
  };

}  // namespace GodotObjectCompiler

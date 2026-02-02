
#pragma once
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class Class;

  class IGodotInitializationGenerator {

    virtual Ref<GeneratorError> generate_initialization(Ref<Class> target_class,
        Ref<GodotClassAttribute> class_attribute, Ref<Context> initialize_content,
        Ref<Context> uninitialize_content) = 0;

    virtual Ref<GeneratorError> generate_startup(Ref<Class> target_class, Ref<GodotClassAttribute> class_attribute,
        Ref<Context> startup_content, Ref<Context> shutdown_content) = 0;
  };

}  // namespace GodotObjectCompiler

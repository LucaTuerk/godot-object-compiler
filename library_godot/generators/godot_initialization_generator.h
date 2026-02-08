
#pragma once
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class Class;

  class IGodotInitializationGenerator {
   public:

    virtual ~IGodotInitializationGenerator() = default;

    virtual Ref<GeneratorError> generate_initialization(Ref<Class> p_target_class,
        Ref<GodotClassAttribute> p_class_attribute, Ref<Context> p_initialize_content,
        Ref<Context> p_uninitialize_content) = 0;

    virtual Ref<GeneratorError> generate_startup(Ref<Class> p_target_class, Ref<GodotClassAttribute> p_class_attribute,
        Ref<Context> p_startup_content, Ref<Context> p_shutdown_content) = 0;
  };

}  // namespace GodotObjectCompiler

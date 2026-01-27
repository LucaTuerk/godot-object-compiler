
#pragma once
#include "../../library/generator/generator.h"
#include "../attributes/attributes_godot.h"
#include "library/attribute_db.h"
#include "library/tree/output/generator_error.h"

namespace GodotObjectCompiler {

  class GodotPropertyGenerator : public IClassGenerator<GodotPropertyAttribute> {
    GENERATOR(GodotPropertyGenerator);

   public:

    GeneratorError* do_generate(Class* target_class, GodotPropertyAttribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) override;

  };

  REGISTER_CLASS_GENERATOR(GodotPropertyGenerator);

}  // namespace GodotObjectCompiler

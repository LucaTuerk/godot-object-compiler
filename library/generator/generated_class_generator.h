
#pragma once
#include "generator.h"
#include "library/tree/output/generator_error.h"
#include "library/tree/syntax/attributes_godot.h"

namespace GodotObjectCompiler {

  class GodotGeneratedClassGenerator : public IGenerator {
    GENERATOR(GodotGeneratedClassGenerator);

   public:
    Context* generate(Context* tree, Node* entry_point) override;

    GeneratorError* generate_property_bindings(Class* _class, GodotPropertyAttribute* property, Context* generated_body,
                                               Context* generated_header);
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "generator.h"

namespace GodotObjectCompiler {

  class MacroIncludeGenerator : public IGenerator {
    GENERATOR(MacroIncludeGenerator);

   public:

    Context* generate(Context* tree, Node* entry_point) override;
  };

}  // namespace GodotObjectCompiler

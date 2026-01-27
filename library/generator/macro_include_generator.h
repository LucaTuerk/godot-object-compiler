#pragma once
#include "generator.h"

namespace GodotObjectCompiler {

  class MacroIncludeGenerator : public IGenerator {
    GENERATOR(MacroIncludeGenerator);

   public:

    Ref<Context> generate(Ref<Context> tree, Ref<Node> entry_point) override;
  };

}  // namespace GodotObjectCompiler

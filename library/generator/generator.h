#pragma once

#include "library/core/core.h"
#include "library/tree/namespace.h"

namespace GodotObjectCompiler {
  class IGenerator {
   public:
    virtual void generate(Context* generation_target, Namespace* tree,
                          Node* entry_point) = 0;
  };
}  // namespace GodotObjectCompiler

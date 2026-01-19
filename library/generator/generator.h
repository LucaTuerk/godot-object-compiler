#pragma once

#include "library/core/core.h"
#include "library/tree/namespace.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {
    class IGenerator {
        public:
			virtual void generate(Context* generation_target, Namespace* tree, Node* entry_point) = 0;
    };
}

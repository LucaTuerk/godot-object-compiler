#pragma once

#include "library/core/core.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {
    class IGenerator {
        public:
            virtual String generate(Node* tree) = 0;
    };
}

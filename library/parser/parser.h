#pragma once

#include "library/core/core.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {
    class IParser {
        public:
            virtual Node* parse(const String& input) = 0;
    };
}

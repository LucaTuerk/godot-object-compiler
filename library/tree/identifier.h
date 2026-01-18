#pragma once

#include "node.h"

namespace GodotObjectCompiler {
    class Identifier : public Node {
        NODE_TYPE(Identifier)

        public:
            virtual void dump_properties(Config& config) override;

        public:
            String name;
    };
}

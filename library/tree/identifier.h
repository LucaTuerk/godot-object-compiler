#pragma once

#include "node.h"

namespace GodotObjectCompiler {
    class Identifier : public Node {
        NODE_TYPE(Identifier)

        public:
            virtual void write_to(IWriter *writer) override;

        public:
            String name;
    };
}

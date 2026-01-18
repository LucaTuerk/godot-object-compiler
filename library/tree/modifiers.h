#pragma once

#include "library/tree/node.h"

namespace GodotObjectCompiler {
    class Virtual : public Node {
        NODE_TYPE(Virtual)
    };

    class Static : public Node {
        NODE_TYPE(Static)
    };

    class Const : public Node {
        NODE_TYPE(Const)
    };

    class Override : public Node {
        NODE_TYPE(Override)
    };
}

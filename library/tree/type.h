#pragma once

#include "node.h"

namespace GodotObjectCompiler {
    class Reference : public Node {
        NODE_TYPE(Reference)
    };

    class Pointer : public Node {
        NODE_TYPE(Pointer)
    };

    class Type : public Context {
        NODE_TYPE(Type)
    };
}

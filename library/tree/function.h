#pragma once

#include "node.h"

namespace GodotObjectCompiler {

    class FunctionParameters : public Context  {
        NODE_TYPE(FunctionParameters)
    };

    class Function : public Context {
        NODE_TYPE(Function)
    };
}

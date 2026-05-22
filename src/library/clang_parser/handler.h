
#pragma once

#include "library/core/core.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler
{
    class IClangNodeHandler
    {
      public:
        virtual ~IClangNodeHandler() = default;
        virtual CXChildVisitResult handle(CXCursor p_cursor, Ref<Context>& p_target) = 0;
        virtual bool handles_node(CXCursor p_cursor) = 0;
    };
} // namespace GodotObjectCompiler

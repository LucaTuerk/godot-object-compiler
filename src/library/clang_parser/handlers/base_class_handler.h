
#pragma once
#include "library/clang_parser/parser.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    class BaseClassHandler : public IClangNodeHandler
    {
        CLANG_HANDLER(BaseClassHandler);

      public:
        bool handles_node(CXCursor p_cursor) override;
        Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) override;
    };

} // namespace GodotObjectCompiler::ClangNodeHandlers

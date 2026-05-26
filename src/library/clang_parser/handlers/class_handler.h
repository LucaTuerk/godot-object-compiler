
#pragma once
#include "library/clang_parser/handler.h"
#include "library/clang_parser/parser.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    class ClassHandler : public IClangNodeHandler
    {
        CLANG_HANDLER(ClassHandler);

      public:
        bool handles_node(CXCursor p_cursor) override;
        Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) override;
    };
} // namespace GodotObjectCompiler::ClangNodeHandlers
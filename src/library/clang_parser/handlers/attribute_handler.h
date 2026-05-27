#pragma once
#include "library/clang_parser/handler.h"
#include "library/clang_parser/parser.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    class AttributeHandler : public IClangASTHandler
    {
        CLANG_AST_HANDLER(AttributeHandler);

      public:
        bool handles_node(CXCursor p_cursor) override;

        Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) override;

        int get_priority() const override;
    };

} // namespace GodotObjectCompiler::ClangASTHandlers
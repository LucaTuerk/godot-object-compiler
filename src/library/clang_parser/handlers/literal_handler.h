
#pragma once
#include "library/clang_parser/handler.h"
#include "library/clang_parser/parser.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    class LiteralHandler : public IClangNodeHandler
    {
        CLANG_HANDLER(LiteralHandler);

      public:
        static bool cursor_is_literal(const CXCursor& cursor);

        static Ref<Literal> cursor_to_literal(const CXCursor& p_cursor);

        bool handles_node(CXCursor p_cursor) override;

        Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) override;
    };

} // namespace GodotObjectCompiler::ClangNodeHandlers


#include "literal_handler.h"

#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{

    bool LiteralHandler::cursor_is_literal(const CXCursor& cursor)
    {
        switch (cursor.kind) {
        case CXCursor_IntegerLiteral:
        case CXCursor_StringLiteral:
            return true;
        default:
            return false;
        }
    }

    Ref<Literal> LiteralHandler::cursor_to_literal(const CXCursor& p_cursor)
    {
        CXEvalResult result = clang_Cursor_Evaluate(p_cursor);
        Ref<Literal> literal = nullptr;

        switch (p_cursor.kind) {
        case CXCursor_IntegerLiteral: {
            const int value = clang_EvalResult_getAsInt(result);
            literal = node_new<Literal>(format("%d", value));
            break;
        }
        case CXCursor_StringLiteral: {
            const char* value = clang_EvalResult_getAsStr(result);
            literal = node_new<Literal>(value);
            break;
        }
        default:
            PANIC("Unhandled literal type");
        }

        clang_EvalResult_dispose(result);
        return literal;
    }

    bool LiteralHandler::handles_node(CXCursor p_cursor)
    {
        return cursor_is_literal(p_cursor);
    }

    IClangNodeHandler::Step
    LiteralHandler::handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        UNUSED(p_root);
        p_target->add_child(cursor_to_literal(p_cursor));
        return Step::Over();
    }
} // namespace GodotObjectCompiler::ClangNodeHandlers
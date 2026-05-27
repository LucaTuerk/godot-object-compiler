#include "attribute_handler.h"

#include "library/core/string_utilities.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    bool is_attribute_name(const String& p_name)
    {
        return string_prefix(p_name, "__GOC_MACRO__");
    }

    bool AttributeHandler::handles_node(CXCursor p_cursor)
    {
        if (!cursor_kind_in(p_cursor, {CXCursor_FieldDecl, CXCursor_VarDecl})) {
            return false;
        }

        const ClangString name = clang_getCursorSpelling(p_cursor);
        return is_attribute_name(name);
    }

    CXCursor find_string_literal(CXCursor p_cursor)
    {
        CXCursor result = clang_getNullCursor();
        clang_visitChildren(p_cursor, [](CXCursor p_cursor, CXCursor p_parent, CXClientData p_result) {
            std::cout << String(ClangString(clang_getCursorKindSpelling(p_cursor.kind))) << std::endl;

            if (p_cursor.kind == CXCursor_StringLiteral) {
                CXCursor* result = static_cast<CXCursor*>(p_result);
                *result = p_cursor;
            }
            return CXChildVisit_Recurse;
        }, &result);
        return result;
    }

    IClangASTHandler::Step
    AttributeHandler::handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        const ClangString name = clang_getCursorSpelling(p_cursor);

        auto arguments_result = clang_Cursor_Evaluate(p_cursor);

        p_target->B<Literal>(name);
        //
        switch (clang_EvalResult_getKind(arguments_result)) {
        case CXEval_StrLiteral: {
            p_target->B<Literal>(format("\"%s\"", clang_EvalResult_getAsStr(arguments_result)));
            break;
        }
        default:
            CXCursor string_literal = find_string_literal(p_cursor);
            clang_EvalResult_dispose(arguments_result);
            arguments_result = clang_Cursor_Evaluate(string_literal);
            p_target->B<Literal>(format("\"%s\"", clang_EvalResult_getAsStr(arguments_result)));
        }

        clang_EvalResult_dispose(arguments_result);
        return Step::Over();
    }

    int AttributeHandler::get_priority() const
    {
        return 1;
    }
} // namespace GodotObjectCompiler::ClangASTHandlers

#include "field_handler.h"

#include "library/tree/syntax/field.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{

    bool FieldHandler::handles_node(CXCursor p_cursor)
    {
        return p_cursor.kind == CXCursor_FieldDecl;
    }

    IClangNodeHandler::Step
    FieldHandler::handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        const ClangString name = clang_getCursorDisplayName(p_cursor);
        const ClangString type_name = clang_getTypeSpelling(clang_getCursorType(p_cursor));

        p_target =
            p_target->B<Field>()[{B<Type>()[{B<Identifier>(type_name)}], B<Identifier>(name)}];
        return Step::Into();
    }
} // namespace GodotObjectCompiler::ClangNodeHandlers
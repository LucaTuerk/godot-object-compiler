
#include "class_handler.h"

#include "library/tree/syntax/class.h"
#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    bool ClassHandler::handles_node(CXCursor p_cursor)
    {
        return p_cursor.kind == CXCursor_ClassDecl;
    }

    IClangNodeHandler::Step
    ClassHandler::handle(const CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        if (p_cursor.kind == CXCursor_ClassDecl) {
            const ClangString name = clang_getCursorDisplayName(p_cursor);
            Ref<Body> body;
            p_root = p_target->B<Class>()[{B<Identifier>(name), B<BaseClasses>(), R<Body>(&body)}];
            p_target = body;
        }
        return Step::Into();
    }
} // namespace GodotObjectCompiler::ClangNodeHandlers

#include "base_class_handler.h"

#include "library/tree/syntax/class.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    bool BaseClassHandler::handles_node(const CXCursor p_cursor)
    {
        return p_cursor.kind == CXCursor_CXXBaseSpecifier;
    }

    IClangNodeHandler::Step
    BaseClassHandler::handle(const CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        Ref<Class> _class = p_root->as<Class>();
        PARSER_ERROR_COND(_class == nullptr, "Invalid root for base class handler");

        Ref<BaseClasses> base_classes = _class->find_child<BaseClasses>();
        PARSER_ERROR_COND(base_classes == nullptr, "Could not find base classes in root class.");

        const ClangString name = clang_getCursorDisplayName(p_cursor);
        base_classes->B<Type>()[{B<Identifier>(name)}];
        return Step::Over();
    }
} // namespace GodotObjectCompiler::ClangNodeHandlers
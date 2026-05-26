
#include "parser.h"

#include "handler.h"
#include "handlers/all.h"
#include "library/tree/predicates.h"

#include <utility>

namespace GodotObjectCompiler
{
    Ref<ParserError> ClangParser::parse(const String& p_input, Ref<Context> r_target)
    {
        UNUSED(p_input);
        UNUSED(r_target);
        PARSER_ERROR("Clang parser can only parse files.");
    }

    Ref<ParserError> ClangParser::parse_file(const String& p_path, Ref<Context> r_target)
    {
        const char* args[] = {"-x", "c++", nullptr};

        CXIndex index = clang_createIndex(0, 0);
        CXTranslationUnit unit = clang_parseTranslationUnit(
            index, p_path.c_str(), args, 2, nullptr, 0, CXTranslationUnit_SkipFunctionBodies);
        PARSER_ERROR_COND(
            unit == nullptr, "Failed to parse translation unit \"%s\"", p_path.c_str());
        const CXCursor root = clang_getTranslationUnitCursor(unit);

        ClangParserContext context{.root = r_target, .current = r_target};
        clang_visitChildren(root, &visitor, &context);

        return ParserError::OK;
    }

    void ClangParser::set_parse_attributes(const bool p_parse_attributes)
    {
        parse_attributes = p_parse_attributes;
    }

    CXChildVisitResult
    ClangParser::visitor(CXCursor p_cursor, CXCursor p_parent, CXClientData p_data)
    {
        using namespace ClangNodeHandlers;
        UNUSED(p_parent);

        ClangParserContext* context = static_cast<ClangParserContext*>(p_data);

#if 1
        const ClangString name = clang_getCursorDisplayName(p_cursor);
        const ClangString kind = clang_getCursorKindSpelling(p_cursor.kind);
        std::cout << static_cast<String>(name) << " " << static_cast<String>(kind) << std::endl;
#endif

        for (const auto& [_, handler] : handlers) {
            if (handler->handles_node(p_cursor)) {
                Ref<Context> current = context->current;
                Ref<Context> root = context->root;

                auto step = handler->handle(p_cursor, current, root);
                if (step.is_into()) {
                    ClangParserContext child_context{
                        .root = root,
                        .current = current,
                    };
                    clang_visitChildren(p_cursor, &visitor, &child_context);
                }
                return CXChildVisit_Continue;
            }
        }
        return CXChildVisit_Continue;
    }

    ClangString::ClangString(CXString p_string) : data(p_string)
    {
        ref_count = new int;
        *ref_count = 1;
    }

    ClangString::ClangString(const ClangString& p_string)
        : ref_count(p_string.ref_count), data(p_string.data)
    {
        *ref_count = *ref_count + 1;
    }

    ClangString::~ClangString()
    {
        *ref_count -= 1;
        if (*ref_count <= 0) {
            delete ref_count;
            clang_disposeString(data);
        }
    }

    ClangString::operator String() const
    {
        return clang_getCString(data);
    }
} // namespace GodotObjectCompiler

#include "parser.h"

#include "handler.h"

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
        const CXIndex index = clang_createIndex(0, 0);
        const CXTranslationUnit unit = clang_parseTranslationUnit(
            index, p_path.c_str(), args, 2, nullptr, 0, CXTranslationUnit_SkipFunctionBodies);
        PARSER_ERROR_COND(
            unit == nullptr, "Failed to parse translation unit \"%s\"", p_path.c_str());

        const CXCursor root = clang_getTranslationUnitCursor(unit);

        static Dictionary<String, Ref<IClangNodeHandler>>& static_handlers = handlers;
        static_handlers = handlers;

        clang_visitChildren(
            root,
            [](CXCursor p_cursor, CXCursor p_parent, CXClientData p_data) {
                for (const auto& [name, handler] : static_handlers) {
                    auto target = *static_cast<Ref<Context>*>(p_data);
                    if (handler->handles_node(p_cursor)) {
                        return handler->handle(p_cursor, target);
                    }
                }
                return CXChildVisit_Continue;
            },
            &r_target);

        return ParserError::OK;
    }

    void ClangParser::set_parse_attributes(const bool p_parse_attributes)
    {
        parse_attributes = p_parse_attributes;
    }
} // namespace GodotObjectCompiler
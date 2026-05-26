
#pragma once
#include "handler.h"
#include "library/parser.h"

namespace GodotObjectCompiler
{
    struct ClangParserContext
    {
        Ref<Context> root;
        Ref<Context> current;
    };

    class ClangParser : public IParser
    {
      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target);

        template <typename T> static bool register_handler(const String& p_name);

        void set_parse_attributes(bool p_parse_attributes);

      private:
        static CXChildVisitResult
        visitor(CXCursor p_cursor, CXCursor p_parent, CXClientData p_data);

        static inline Dictionary<String, Ref<ClangNodeHandlers::IClangNodeHandler>> handlers;

        bool parse_attributes = true;
    };

    template <typename T> bool ClangParser::register_handler(const String& p_name)
    {
        auto [itr, success] = handlers.emplace(p_name, new T());
        return success;
    }

    class ClangString
    {
      public:
        ClangString(CXString p_string);
        ClangString(const ClangString& p_string);
        ~ClangString();

        operator String() const;

      private:
        int* ref_count;
        CXString data{};
    };
} // namespace GodotObjectCompiler

#define CLANG_HANDLER(type)                                                                        \
    static inline bool __handler_registered__ = ClangParser::register_handler<type>(#type)

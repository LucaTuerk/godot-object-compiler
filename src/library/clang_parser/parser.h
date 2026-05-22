
#pragma once
#include "library/parser.h"
#include "node_handler.h"

namespace GodotObjectCompiler
{
    class ClangParser : public IParser
    {

      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target);

        template <typename T> bool register_handler(const String& p_name);

        void set_parse_attributes(bool p_parse_attributes);

      private:
        Dictionary<String, Ref<IClangNodeHandler>> handlers;
        bool parse_attributes = true;
    };

    template <typename T> bool ClangParser::register_handler(const String& p_name)
    {
        auto [itr, success] = handlers.emplace(p_name, new T());
        return success;
    }
} // namespace GodotObjectCompiler

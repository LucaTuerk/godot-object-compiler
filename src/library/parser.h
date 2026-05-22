#pragma once
#include "tree/syntax/parser_error.h"

namespace GodotObjectCompiler
{
    class IParser
    {
      public:
        virtual ~IParser() = default;
        virtual Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) = 0;
    };
} // namespace GodotObjectCompiler
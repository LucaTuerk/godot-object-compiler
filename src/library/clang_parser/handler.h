
#pragma once

#include "library/core/core.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{
    class IClangNodeHandler
    {
      public:
        class Step
        {
          public:
            enum Type { StepOver, StepInto, Error };

            static Step Into();
            static Step Over();
            Step(const Ref<ParserError>& p_error) : type(Error), error(p_error)
            {
            }

            [[nodiscard]] bool is_into() const;
            [[nodiscard]] bool is_over() const;
            [[nodiscard]] bool is_error() const;
            Ref<ParserError> get_error();

          private:
            Step() = default;

            Type type = StepOver;
            Ref<ParserError> error = nullptr;
        };

        virtual ~IClangNodeHandler() = default;
        virtual bool handles_node(CXCursor p_cursor) = 0;
        virtual Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) = 0;
    };

} // namespace GodotObjectCompiler::ClangNodeHandlers

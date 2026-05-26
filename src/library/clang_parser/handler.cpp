#include "handler.h"

namespace GodotObjectCompiler::ClangNodeHandlers
{

    IClangNodeHandler::Step IClangNodeHandler::Step::Into()
    {
        Step step;
        step.type = StepInto;
        step.error = nullptr;
        return step;
    }

    IClangNodeHandler::Step IClangNodeHandler::Step::Over()
    {
        Step step;
        step.type = StepOver;
        step.error = nullptr;
        return step;
    }

    bool IClangNodeHandler::Step::is_into() const
    {
        return type == StepInto;
    }

    bool IClangNodeHandler::Step::is_over() const
    {
        return type == StepOver;
    }

    bool IClangNodeHandler::Step::is_error() const
    {
        return type == Error;
    }

    Ref<ParserError> IClangNodeHandler::Step::get_error()
    {
        return error;
    }
} // namespace GodotObjectCompiler::ClangNodeHandlers
/**************************************************************************/
/* literal_handler.cpp                                                    */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "literal_handler.h"

#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler::ClangASTHandlers
{

    bool LiteralHandler::cursor_is_literal(const CXCursor& cursor)
    {
        return cursor_kind_in(
            cursor, {
                        CXCursor_IntegerLiteral,
                        CXCursor_FloatingLiteral,
                        CXCursor_StringLiteral,
                        CXCursor_UnexposedExpr,
                        CXCursor_BinaryOperator,
                    });
    }

    Result<Literal, ParserError> LiteralHandler::cursor_to_literal(const CXCursor& p_cursor)
    {
        CXEvalResult result = clang_Cursor_Evaluate(p_cursor);
        Ref<Literal> literal = nullptr;

        switch (clang_EvalResult_getKind(result)) {
        case CXEval_Int: {
            literal = node_new<Literal>(format("%d", clang_EvalResult_getAsInt(result)));
            break;
        }
        case CXEval_Float: {
            literal = node_new<Literal>(format("%f", clang_EvalResult_getAsDouble(result)));
            break;
        }
        case CXEval_StrLiteral: {
            literal = node_new<Literal>(format("\"%s\"", clang_EvalResult_getAsStr(result)));
            break;
        }
        default:
            literal = node_new<Literal>(get_cursor_spelling(p_cursor));
            break;
        }

        clang_EvalResult_dispose(result);
        return literal;
    }

    bool LiteralHandler::handles_cursor(CXCursor p_cursor)
    {
        return cursor_is_literal(p_cursor);
    }

    IClangASTHandler::Step
    LiteralHandler::handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        UNUSED(p_root);
        Result<Literal, ParserError> literal_result = cursor_to_literal(p_cursor);
        RESULT_ERROR_PASS_ON(ParserError, literal_result, literal);

        p_target->add_child(literal);
        return Step::Over();
    }
} // namespace GodotObjectCompiler::ClangASTHandlers
/**************************************************************************/
/* attribute_handler.cpp                                                  */
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
#include "attribute_handler.h"

#include "library/core/string_utilities.h"
#include "library/parsers/common.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    bool is_attribute_name(const String& p_name)
    {
        return string_prefix(p_name, "__GOC_MACRO__");
    }

    bool AttributeHandler::handles_cursor(const CXCursor& p_cursor)
    {
        if (!cursor_kind_in(p_cursor, {CXCursor_FieldDecl, CXCursor_VarDecl})) {
            return false;
        }

        const ClangString name = clang_getCursorSpelling(p_cursor);
        return is_attribute_name(name);
    }

    CXCursor find_string_literal(const CXCursor& p_cursor)
    {
        CXCursor result = clang_getNullCursor();
        clang_visitChildren(
            p_cursor,
            [](CXCursor p_cursor, CXCursor p_parent, CXClientData p_result) {
                UNUSED(p_parent);
                if (p_cursor.kind == CXCursor_StringLiteral) {
                    auto result = static_cast<CXCursor*>(p_result);
                    *result = p_cursor;
                }
                return CXChildVisit_Recurse;
            },
            &result);
        return result;
    }

    IClangASTHandler::Step
    AttributeHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        UNUSED(p_root);
        const ClangString name = clang_getCursorSpelling(p_cursor);

        String arguments_content = "";

        if (ClangEvalResult arguments_result = clang_Cursor_Evaluate(p_cursor);
            clang_EvalResult_getKind(arguments_result) == CXEval_StrLiteral) {
            arguments_content = format("%s", clang_EvalResult_getAsStr(arguments_result));
        } else {
            const CXCursor string_literal = find_string_literal(p_cursor);
            ClangEvalResult result = clang_Cursor_Evaluate(string_literal);
            arguments_content = format("%s", clang_EvalResult_getAsStr(result));
        }

        const Vector<String> parts = string_split(name, "__");
        CLANG_PARSER_ERROR_COND(
            parts.size() != 4, "Unexpected attribute \"%s\" can not be parsed.",
            String(name).c_str());
        const String& macro = parts[2];

        const SourceLocation location{
            context->cursor_start_offset(p_cursor),
            context->cursor_end_offset(p_cursor),
            context->cursor_start_line(p_cursor),
        };

        const auto attribute_result = ParserUtilities::parse_attribute(
            macro, arguments_content, location, context->parse_attributes);

        if (attribute_result.has_error()) {
            attribute_result.get_error()->set_handled();
            CLANG_PARSER_ERROR(attribute_result.get_error()->message.c_str());
        }

        p_target->add_child(attribute_result.get_result());
        return Step::Over();
    }

    int AttributeHandler::get_priority() const
    {
        return 1;
    }
} // namespace GodotObjectCompiler::ClangASTHandlers
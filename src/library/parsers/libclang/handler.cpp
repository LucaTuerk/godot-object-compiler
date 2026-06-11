/**************************************************************************/
/* handler.cpp                                                            */
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
#include "handler.h"

#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/modifiers.h"
#include "parser.h"

namespace GodotObjectCompiler::ClangASTHandlers
{

    IClangASTHandler::Step IClangASTHandler::Step::Into()
    {
        Step step;
        step.type = StepInto;
        step.error = nullptr;
        return step;
    }

    IClangASTHandler::Step IClangASTHandler::Step::Over()
    {
        Step step;
        step.type = StepOver;
        step.error = nullptr;
        return step;
    }

    bool IClangASTHandler::Step::is_into() const
    {
        return type == StepInto;
    }

    bool IClangASTHandler::Step::is_over() const
    {
        return type == StepOver;
    }

    bool IClangASTHandler::Step::is_error() const
    {
        return type == Error;
    }

    Ref<ParserError> IClangASTHandler::Step::get_error()
    {
        return error;
    }

    int IClangASTHandler::get_priority() const
    {
        return 0;
    }

    bool IClangASTHandler::cursor_kind_in(
        const CXCursor& p_cursor, const std::initializer_list<CXCursorKind>& p_kinds)
    {
        return std::any_of(p_kinds.begin(), p_kinds.end(), [&p_cursor](const auto& kind) {
            if (kind == p_cursor.kind) {
                return true;
            }
            return false;
        });
    }

    String IClangASTHandler::get_cursor_spelling(const CXCursor& p_cursor)
    {
        const CXSourceRange range = clang_getCursorExtent(p_cursor);
        const CXTranslationUnit unit = clang_Cursor_getTranslationUnit(p_cursor);
        CXToken* tokens = nullptr;
        unsigned num_tokens = 0;
        clang_tokenize(unit, range, &tokens, &num_tokens);

        if (num_tokens > 0) {
            ClangString spelling = clang_getTokenSpelling(unit, tokens[0]);
            clang_disposeTokens(unit, tokens, num_tokens);
            return spelling;
        }

        clang_disposeTokens(unit, tokens, num_tokens);
        return "";
    }

    Result<Type, ParserError> IClangASTHandler::get_cursor_type(const CXCursor& p_cursor)
    {
        CXType type = clang_getCursorType(p_cursor);
        if (cursor_kind_in(p_cursor, {CXCursor_FunctionDecl, CXCursor_CXXMethod})) {
            type = clang_getResultType(type);
        }
        const ClangString type_name = clang_getTypeSpelling(type);
        return get_type_from_spelling(type_name);
    }

    bool type_is_const(String& p_spelling)
    {
        if (string_prefix(p_spelling, "const ")) {
            p_spelling = p_spelling.substr(6);
            return true;
        }
        return false;
    }

    Vector<Ref<Node>> get_modifiers(String& p_spelling)
    {
        Vector<Ref<Node>> modifiers;
        int mod_start = std::numeric_limits<int>::max();
        int i = 0;
        for (const char c : p_spelling) {
            if (c == '&') {
                mod_start = std::min(mod_start, i);
                modifiers.push_back(node_new<Reference>());
            }
            if (c == '*') {
                mod_start = std::min(mod_start, i);
                modifiers.push_back(node_new<Pointer>());
            }
            ++i;
        }

        if (mod_start != std::numeric_limits<int>::max()) {
            p_spelling = string_trim(p_spelling.substr(0, mod_start - 1));
        }
        return modifiers;
    }

    Result<Type, ParserError> IClangASTHandler::get_type_from_spelling(const String& p_spelling)
    {
        String spelling = p_spelling;

        const Vector<String> template_args =
            string_get_enclosed_sections(p_spelling, '<', '>', ',');
        Ref<TemplateArguments> arguments = nullptr;

        if (template_args.size() > 1) {
            arguments = node_new<TemplateArguments>();

            for (Size i = 0; i < template_args.size(); ++i) {
                if (i == 0) {
                    spelling = template_args[i];
                } else {
                    auto type_result = get_type_from_spelling(template_args[i]);
                    RESULT_ERROR_PASS_ON(ParserError, type_result, type);
                    arguments->add_child(type);
                }
            }
        }

        Ref<Type> result = B<Type>();

        if (type_is_const(spelling)) {
            result->B<Const>();
        }

        const Ref<Identifier> identifier = result->B<Identifier>();

        for (const auto& modifier : get_modifiers(spelling)) {
            result->add_child(modifier);
        }

        identifier->name = spelling;

        if (arguments) {
            result->add_child(arguments);
        }
        return result;
    }

    String IClangASTHandler::get_cursor_assigned_value(const CXCursor& p_cursor)
    {
        const CXSourceRange range = clang_getCursorExtent(p_cursor);
        const CXTranslationUnit unit = clang_Cursor_getTranslationUnit(p_cursor);
        CXToken* tokens = nullptr;
        unsigned num_tokens = 0;
        clang_tokenize(unit, range, &tokens, &num_tokens);

        if (num_tokens > 0) {
            StreamWriter writer;
            bool record = false;
            for (unsigned i = 0; i < num_tokens; ++i) {
                const String spelling = ClangString(clang_getTokenSpelling(unit, tokens[i]));
                if (spelling == ";" || spelling == ",") {
                    break;
                }

                if (spelling == "=") {
                    record = true;
                    continue;
                }

                if (!record) {
                    continue;
                }

                if (writer.current_length() != 0) {
                    writer.write(" ");
                }
                writer.write(spelling);
            }

            clang_disposeTokens(unit, tokens, num_tokens);
            return writer.get_string();
        }

        clang_disposeTokens(unit, tokens, num_tokens);
        return "";
    }

} // namespace GodotObjectCompiler::ClangASTHandlers
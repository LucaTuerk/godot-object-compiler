/**************************************************************************/
/* parser.cpp                                                             */
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

        ClangParserContext context{.root = r_target, .current = r_target, .unit = unit};
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
        using namespace ClangASTHandlers;
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
                        .root = root, .current = current, .unit = context->unit};
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
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

#include "application/application_context.h"
#include "handler.h"
#include "handlers/all.h"
#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/core/temp_file.h"
#include "library/parsers/common.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/include.h"
#include "library/tree/syntax/namespace.h"

#define CLANG_PARSER_PRINT_DEBUG 0

namespace GodotObjectCompiler
{
    Pair<String, String> split_lines_include(const String& p_input)
    {
        Size last_include = p_input.rfind("#include");
        if (last_include == String::npos) {
            return {"", p_input};
        }

        Size last_include_new_line = p_input.find('\n', last_include);
        return {
            p_input.substr(0, last_include_new_line), p_input.substr(last_include_new_line + 1)};
    }

    Size line_count(const String& p_input)
    {
        Size lines = 1;
        for (const char c : p_input) {
            if (c == '\n') {
                lines++;
            }
        }
        return lines;
    }

    Vector<Ref<Include>> parse_includes(const String& p_input)
    {
        Vector<Ref<Include>> includes;

        for (const auto& line : string_split(p_input, "\n")) {
            const Size incl = line.find("#include");
            if (incl == String::npos) {
                continue;
            }

            bool brace_open = false, chevron_open = false;
            StreamWriter writer;

            for (Size i = incl + 8; i < line.size(); ++i) {
                if (line[i] == '"') {
                    if (!brace_open) {
                        brace_open = true;
                    } else {
                        break;
                    }
                } else if (line[i] == '<') {
                    if (!chevron_open) {
                        chevron_open = true;
                    }
                } else if (line[i] == '>' && chevron_open) {
                    break;
                } else if (chevron_open || brace_open) {
                    writer.write_generic(line[i]);
                }
            }

            if (writer.current_length() != 0) {
                includes.push_back(node_new<Include>(writer.get_string(), chevron_open));
            }
        }
        return includes;
    }

    Vector<Ref<Define>> parse_defines(const String& p_input)
    {
        Vector<Ref<Define>> defines;
        Size current = 0;
        Size next = 0;

        while (true) {
            current = p_input.find("#define", next);
            next = current + 1;

            if (current == String::npos) {
                break;
            }

            bool valid = true;
            for (Size i = current - 1; current != 0 && i != 0; --i) {
                if (p_input[i] == '\n') {
                    break;
                }

                if (!is_whitespace(p_input[i])) {
                    valid = false;
                    break;
                }
            }

            if (!valid) {
                continue;
            }

            StreamWriter writer;
            bool record = false;
            for (Size i = current + 7; i < p_input.size(); ++i) {
                if (!record && p_input[i] != ' ') {
                    record = true;
                }

                if (record) {
                    if (!isalnum(p_input[i]) && p_input[i] != '_') {
                        break;
                    }

                    writer.write_generic(p_input[i]);
                }
            }
            if (writer.current_length() != 0) {
                Ref<Define> define = B<Define>()[{B<Identifier>(writer.get_string())}];
                defines.push_back(define);
            }
        }

        return defines;
    }

    Ref<ParserError> ClangParser::parse(const String& p_input, Ref<Context> r_target)
    {
        PARSER_ERROR_COND(
            !r_target->is<Namespace>(),
            "Invalid target node. Expected to be the global namespace.");
        PARSER_ERROR_COND(
            r_target->get_child_count() > 0, "Invalid target node. Expected to be the empty.");

        const String local_input = ParserUtilities::strip_excluded_sections(p_input);

        Permissions::instance()->add_write_path(path_cwd());

        Size added_characters = 0, added_lines = 0;
        Ref<Body> body = r_target->B<Body>();

        StreamWriter writer;

        auto parts = split_lines_include(local_input);

        for (const auto& define : parse_defines(local_input)) {
            body->add_child(define);
        }

        for (const auto& include : parse_includes(parts.first)) {
            body->add_child(include);
        }

        writer.write(parts.first);
        writer.write("\nint __GOC_CLANG_PARSER_START = 0;\n");

        auto macros = LibraryContext::instance()->get_attribute_db()->get_all_macros();
        writer.write("#define MERGE_INNER(a,b) a##b\n");
        writer.write("#define MERGE(a,b) MERGE_INNER(a,b)\n");
        for (const auto& macro : macros) {
            writer.write(format("#undef %s\n", macro.c_str()));
            writer.write(format(
                "#define %s(...) const char* MERGE(__GOC_MACRO__%s__, __LINE__) = "
                "#__VA_ARGS__;\n",
                macro.c_str(), macro.c_str()));
        }

        writer.write(parts.second);

        added_characters = writer.current_length() - p_input.size();
        added_lines += line_count(writer.get_string()) - line_count(p_input);

#if CLANG_PARSER_PRINT_DEBUG
        std::cout << writer.get_string();
#endif

        TempFile temp_file("h", writer.get_string());

        Vector<const char*> args = {"-x", "c++"};
        Vector<String> include_args;
        Vector<String> includes = LibraryContext::instance()->get_include_paths();

        if (current_file.has_value()) {
            includes.push_back(path_base(*current_file));
            current_file = {};
        }

        for (const auto& include_path : includes) {
            String arg = format("-I%s", path_absolute(include_path).c_str());
            include_args.emplace_back(format("-I%s", path_absolute(include_path).c_str()));
            args.push_back(include_args.back().c_str());
        }

        CXIndex index = clang_createIndex(0, 0);
        CXTranslationUnit unit = clang_parseTranslationUnit(
            index, String(temp_file).c_str(), args.data(), static_cast<int>(args.size()), nullptr,
            0, CXTranslationUnit_SkipFunctionBodies);
        PARSER_ERROR_COND(unit == nullptr, "Failed to parse translation unit.");
        const CXCursor root = clang_getTranslationUnitCursor(unit);

        ClangParserContext context{
            .root = r_target,
            .current = body,
            .unit = unit,
            .parse_attributes = parse_attributes,
            .added_lines = added_lines,
            .added_characters = added_characters};

        clang_visitChildren(root, &visitor, &context);

        if (context.error != nullptr) {
            return context.error;
        }
        return ParserError::OK;
    }

    Ref<ParserError> ClangParser::parse_file(const String& p_path, Ref<Context> r_target)
    {
        current_file = path_absolute(p_path);
        return parse(read_file(p_path), r_target);
    }

    int ClangParser::get_capabilities()
    {
        return CAPABILITIES_SOURCE_PARSER | CAPABILITIES_SOURCE_SUPPORT_MACRO_EXPANSION;
    }

    void ClangParser::config(Config p_config)
    {
        switch (p_config) {
        case CONFIG_PARSE_ATTRIBUTES:
            parse_attributes = true;
            break;
        case CONFIG_SKIP_ATTRIBUTES:
            parse_attributes = false;
            break;
        }
    }

#define VISITOR_ERROR_BREAK(context, err)                                                          \
    do {                                                                                           \
        if (err != nullptr) {                                                                      \
            context->error = err;                                                                  \
            return CXChildVisit_Break;                                                             \
        }                                                                                          \
    } while (false)

    CXChildVisitResult
    ClangParser::visitor(CXCursor p_cursor, CXCursor p_parent, CXClientData p_data)
    {
        using namespace ClangASTHandlers;
        UNUSED(p_parent);

        auto* context = static_cast<ClangParserContext*>(p_data);

        if (!context->active) {
            if (p_cursor.kind == CXCursor_VarDecl &&
                String(ClangString(clang_getCursorSpelling(p_cursor))) ==
                    "__GOC_CLANG_PARSER_START") {
                context->active = true;
            }
            return CXChildVisit_Continue;
        }

#if CLANG_PARSER_PRINT_DEBUG
        const ClangString name = clang_getCursorDisplayName(p_cursor);
        const ClangString kind = clang_getCursorKindSpelling(p_cursor.kind);
        std::cout << static_cast<String>(name) << " " << static_cast<String>(kind) << std::endl;
#endif

        for (const auto& handler : handlers) {
            handler->context = context;
            if (handler->handles_cursor(p_cursor)) {
                Ref<Context> current = context->current;
                Ref<Context> root = context->root;

                auto step = handler->handle(p_cursor, current, root);
                if (step.is_into()) {
                    ClangParserContext child_context{
                        .active = context->active,
                        .root = root,
                        .current = current,
                        .unit = context->unit,
                        .parse_attributes = context->parse_attributes,
                        .added_lines = context->added_lines,
                        .added_characters = context->added_characters};

                    clang_visitChildren(p_cursor, &visitor, &child_context);
                    VISITOR_ERROR_BREAK(context, child_context.error);
                }

                VISITOR_ERROR_BREAK(context, step.get_error());
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
        if (*ref_count == 0) {
            delete ref_count;
            clang_disposeString(data);
        }
    }

    ClangString::operator String() const
    {
        return clang_getCString(data);
    }
} // namespace GodotObjectCompiler
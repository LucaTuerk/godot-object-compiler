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
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/include.h"
#include "library/tree/syntax/namespace.h"
#include "parser_context.h"

namespace GodotObjectCompiler
{
    Pair<String, String> split_lines_include(const String& p_input)
    {
        const Size last_include = p_input.rfind("#include");
        if (last_include == String::npos) {
            return {"", p_input};
        }

        const Size last_include_new_line = p_input.find('\n', last_include);
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

    Pair<Size, Size> get_define_content_range(const String& p_line, const Size& p_start)
    {
        bool record = false;
        Size begin = 0;
        Size end = p_line.size();

        for (Size i = p_start + 8; i < p_line.size(); ++i) {
            if (!record && p_line[i] != ' ') {
                record = true;
                begin = i;
            }

            if (record && !isalnum(p_line[i]) && p_line[i] != '_') {
                end = i;
                break;
            }
        }

        if (begin >= end) {
            return {0, 0};
        }
        return {begin, end};
    }

    Pair<Size, Size>
    get_include_content_range(const String& p_line, const Size& p_start, bool& r_is_system_include)
    {
        bool brace_open = false;
        bool chevron_open = false;
        Size begin = 0;
        Size end = 0;

        for (Size i = p_start + 9; i < p_line.size(); ++i) {
            if (p_line[i] == '"') {
                if (!brace_open) {
                    begin = i + 1;
                    r_is_system_include = false;
                    brace_open = true;
                } else {
                    end = i;
                    break;
                }
            } else if (p_line[i] == '<') {
                if (!chevron_open) {
                    begin = i + 1;
                    r_is_system_include = true;
                    chevron_open = true;
                }
            } else if (p_line[i] == '>' && chevron_open) {
                end = i;
                break;
            }
        }

        if (begin >= end) {
            return {0, 0};
        }
        return {begin, end};
    }

    Vector<Ref<Node>> parse_includes_and_defines(const String& p_input)
    {
        Vector<Ref<Node>> results;
        std::istringstream stream(p_input);

        for (String line; std::getline(stream, line);) {
            if (const Size def = line.find("#define "); def != String::npos) {
                StreamWriter writer;

                auto [begin, end] = get_define_content_range(line, def);
                if (begin == end) {
                    continue;
                }
                auto define_name = line.substr(begin, end - begin);
                Ref<Define> define = B<Define>()[{B<Identifier>(define_name)}];
                results.push_back(define);
            } else if (const Size incl = line.find("#include "); incl != String::npos) {
                bool is_system_include = false;
                auto [begin, end] = get_include_content_range(line, incl, is_system_include);
                if (begin == end) {
                    continue;
                }
                auto include_path = line.substr(begin, end - begin);
                results.push_back(node_new<Include>(include_path, is_system_include));
            }
        }
        return results;
    }

    Ref<ParserError> ClangParser::parse(const String& p_input, Ref<Context> r_target)
    {
        static CXIndex index = clang_createIndex(0, 0);

        PARSER_ERROR_COND(
            !r_target->is<Namespace>(),
            "Invalid target node. Expected to be the global namespace.");
        PARSER_ERROR_COND(
            r_target->get_child_count() > 0, "Invalid target node. Expected to be the empty.");

        const String local_input = ParserUtilities::strip_excluded_sections(p_input);

        Size added_characters = 0;
        Size added_lines = 0;
        Size first_line_added = 0;
        Size fist_character_added = 0;
        Ref<Body> body = r_target->B<Body>();

        StreamWriter writer;

        for (const auto& incl_or_def : parse_includes_and_defines(local_input)) {
            body->add_child(incl_or_def);
        }

        auto [include_section, post_include_section] = split_lines_include(local_input);
        writer.write(include_section);
        fist_character_added = include_section.size();
        first_line_added = line_count(writer.get_string()) + 1;

        StreamWriter added_section_writer;
        added_section_writer.write("\nint __GOC_CLANG_PARSER_START = 0;\n");

        auto macros = LibraryContext::instance()->get_attribute_db()->get_all_macros();
        added_section_writer.write("#define MERGE_INNER(a,b) a##b\n");
        added_section_writer.write("#define MERGE(a,b) MERGE_INNER(a,b)\n");

        for (const auto& macro : macros) {
            added_section_writer.write(format("#undef %s\n", macro.c_str()));
            added_section_writer.write(format(
                "#define %s(...) const char* MERGE(__GOC_MACRO__%s__, __LINE__) = "
                "#__VA_ARGS__;\n",
                macro.c_str(), macro.c_str()));
        }

        writer.write(added_section_writer.get_string());
        writer.write(post_include_section);

        added_characters = added_section_writer.current_length();
        added_lines += line_count(added_section_writer.get_string()) - 2;

        String contents = writer.get_string();

        // TODO: Allow setting C++ standard as application argument
        Vector<const char*> args = {"-x", "c++", "-std=c++17"};
        Vector<String> include_args;
        Vector<String> includes = LibraryContext::instance()->get_include_paths();

        for (const auto& include_path : includes) {
            include_args.emplace_back(format("-I%s", include_path.c_str()));
            args.push_back(include_args.back().c_str());
        }

        TempFile temp_file("h", contents);
        String file_path;
        if (current_file.has_value()) {
            file_path = *current_file;
            current_file = {};
            include_args.emplace_back(format("-I%s", path_base(file_path).c_str()));
            args.push_back(include_args.back().c_str());
        }

        ClangTranslationUnit unit = clang_parseTranslationUnit(
            index, temp_file.get_path().c_str(), args.data(), static_cast<int>(args.size()),
            nullptr, 0,
            CXTranslationUnit_SkipFunctionBodies |
                CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles);

        PARSER_ERROR_COND(unit == nullptr, "Failed to parse source file \"%s\"", file_path.c_str());

        ClangParserContext context{
            false,
            nullptr,
            r_target,
            body,
            unit,
            parse_attributes,
            added_lines,
            added_characters,
            fist_character_added,
            first_line_added,
            file_path,
            p_input,
        };

        for (unsigned i = 0; i < clang_getNumDiagnostics(unit); ++i) {
            if (ClangDiagnostic diagnostic = clang_getDiagnostic(unit, i);
                clang_getDiagnosticSeverity(diagnostic) >= CXDiagnostic_Error) {
                StreamWriter error_writer;
                ClangString spelling =
                    clang_formatDiagnostic(diagnostic, CXDiagnostic_DisplayCategoryName);

                CXFile file;
                unsigned line;

                CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
                clang_getFileLocation(location, &file, &line, nullptr, nullptr);

                if (!path_equals(ClangString(clang_getFileName(file)), temp_file.get_path())) {
                    // Skip errors in included files.
                    continue;
                }

                if (string_contains(spelling, ".generated.h")) {
                    // Skip error, file is not yet generated
                    continue;
                }

                error_writer.write("\n");
                error_writer.write(spelling);

                error_writer.write("\n");
                error_writer.write(string_extract_lines(
                    p_input, context.line_temp_to_original(line - 3),
                    context.line_temp_to_original(line + 3), context.line_temp_to_original(line)));

                PARSER_ERROR(
                    "ClangParser: %s%s", file_path.c_str(), error_writer.get_string().c_str());
            }
        }

        const CXCursor root = clang_getTranslationUnitCursor(unit);

        clang_visitChildren(root, &visitor, &context);

        if (context.error != nullptr) {
            return context.error;
        }
        return ParserError::OK;
    }

    Ref<ParserError> ClangParser::parse_file(const String& p_path, const Ref<Context> r_target)
    {
        current_file = path_absolute(p_path);
        return parse(read_file(p_path), r_target);
    }

    void ClangParser::config(const Config p_config)
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

        for (const auto& handler : handlers) {
            handler->context = context;

            if (handler->handles_cursor(p_cursor)) {
                Ref<Context> current = context->current;
                Ref<Context> root = context->root;

                auto step = handler->handle(p_cursor, current, root);
                if (step.is_into()) {
                    const Ref<Context> parent_root = context->root;
                    const Ref<Context> parent_current = context->current;

                    context->root = root;
                    context->current = current;

                    clang_visitChildren(p_cursor, &visitor, context);

                    context->root = parent_root;
                    context->current = parent_current;
                    VISITOR_ERROR_BREAK(context, context->error);
                }

                VISITOR_ERROR_BREAK(context, step.get_error());
                return CXChildVisit_Continue;
            }
        }
        return CXChildVisit_Continue;
    }

    ClangString::operator String() const
    {
        return clang_getCString(_data);
    }
} // namespace GodotObjectCompiler

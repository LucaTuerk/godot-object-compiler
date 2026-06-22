/**************************************************************************/
/* parser_error.cpp                                                       */
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

#include "parser_error.h"

#include "context.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/parsers/libclang/parser.h"

#if GOC_TREE_SITTER_PARSER_ENABLED
#include "library/parsers/tree-sitter/tree_sitter_node.h"
#endif
#if GOC_LIBCLANG_PARSER_ENABLED
#include "library/parsers/libclang/parser_context.h"
#endif

namespace GodotObjectCompiler
{

    Error::~Error()
    {
        if (!handled && LibraryContext::instance()->get_error_level() >= error_level) {
            print_err(message);
        }
    }

    String Error::to_string() const
    {
        return get_type_static();
    }

    bool Error::copy_to(const Ref<Node>& p_other) const
    {
        COPY_GUARD(ParserError, Node);
        target->message = message;
        return true;
    }

    void Error::write_to(IStructuredWriter* p_writer)
    {
        Node::write_to(p_writer);
        p_writer->write("error_offending", message);
    }

    void Error::read_from(IStructuredReader* p_reader)
    {
        Node::read_from(p_reader);
        p_reader->read<String, String>("error_offending");
    }

    void Error::set_handled()
    {
        handled = true;
    }

    GeneratorError::GeneratorError(
        ErrorLevel level, const String& generator_name, const String& user_message, Ref<Node> node)
    {
        error_level = level;
        StreamWriter writer;
        writer.write(error_level_to_string(level));
        writer.write(" ");
        writer.write(generator_name);
        writer.write(": ");
        writer.write(user_message);
        writer.write("\n");

        if (LibraryContext::instance()->get_error_detail() == ErrorDetail::FULL) {
            writer.write("\nOccurred while processing node:\n");

            if (Ref<Context> parent = node->get_parent()) {
                Size line = 0;
                String pretty = parent->print_pretty_and_get_child_line(node, line);

                Ref<Context> context = node->as<Context>();
                Size node_lines = context ? context->get_descendant_count() : 1;

                writer.write(string_extract_lines(
                    pretty, line - std::min(line, static_cast<Size>(3)), line + node_lines + 3,
                    line));
            } else {
                writer.write(string_extract_lines(node->pretty_print(), 0, 6, 1));
            }
        }

        writer.write("\n");
        message = writer.get_string();
    }

#if GOC_TREE_SITTER_PARSER_ENABLED
    ParserError::ParserError(
        ErrorLevel p_level, const Ref<TreeSitterNode>& p_node, const String& p_message)
        : ParserError(
              p_level, "TreeSitterParser", p_message, p_node->context->file_path,
              p_node->context->buffer, p_node->start_point.row + 1, p_node->start_point.column + 1)
    {
    }
#endif

#if GOC_LIBCLANG_PARSER_ENABLED
    ParserError::ParserError(
        ErrorLevel p_level, const ClangParserContext* p_context, const CXCursor& p_cursor,
        const String& p_message)
        : ParserError(
              p_level, "ClangParser", p_message, p_context->file_path, p_context->original_content,
              p_context->cursor_start_line(p_cursor), p_context->cursor_column(p_cursor))
    {
    }
#endif

    ParserError::ParserError(
        ErrorLevel p_level, const String& p_parser_name, const String& user_message,
        const String& p_file_path, const String& p_file_content, Size p_line, Size p_column)
    {
        error_level = p_level;
        StreamWriter writer;
        writer.write(error_level_to_string(p_level));
        writer.write(" ");
        writer.write(p_parser_name);
        writer.write(": ");
        writer.write(p_file_path);
        writer.write(":");
        writer.write_generic(p_line);
        writer.write(":");
        writer.write_generic(p_column);
        writer.write(" ");
        writer.write("\n");
        writer.write(user_message);
        writer.write("\n");

        if (LibraryContext::instance()->get_error_detail() == ErrorDetail::FULL) {
            writer.write("\nOccurred while processing source:\n");
            writer.write(string_extract_lines(
                p_file_content, p_line - std::min(p_line, static_cast<Size>(3)), p_line + 3,
                p_line));
        } else {
            writer.write(string_extract_lines(p_file_content, p_line, p_line, p_line));
        }

        message = writer.get_string();
    }

} // namespace GodotObjectCompiler

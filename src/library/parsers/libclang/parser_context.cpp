/**************************************************************************/
/* parser_context.cpp                                                     */
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

#include "parser_context.h"

namespace GodotObjectCompiler
{
    Size ClangParserContext::line_temp_to_original(const Size temp_line) const
    {
        return temp_line < first_line_added + added_lines ? temp_line : temp_line - added_lines;
    }

    Size ClangParserContext::offset_temp_to_original(const Size temp_offset) const
    {
        return temp_offset < first_character_added + added_characters
                   ? temp_offset
                   : temp_offset - added_characters;
    }

    Size ClangParserContext::cursor_start_line(const CXCursor& cursor) const
    {
        const CXSourceRange extent = clang_getCursorExtent(cursor);
        CXSourceLocation location = clang_getRangeStart(extent);
        unsigned line;
        clang_getFileLocation(location, nullptr, &line, nullptr, nullptr);
        return line_temp_to_original(line);
    }

    Size ClangParserContext::cursor_end_line(const CXCursor& cursor) const
    {
        const CXSourceRange extent = clang_getCursorExtent(cursor);
        CXSourceLocation location = clang_getRangeEnd(extent);
        unsigned line;
        clang_getFileLocation(location, nullptr, &line, nullptr, nullptr);
        return line_temp_to_original(line);
    }

    Size ClangParserContext::cursor_start_offset(const CXCursor& cursor) const
    {
        const CXSourceRange extent = clang_getCursorExtent(cursor);
        CXSourceLocation location = clang_getRangeStart(extent);
        unsigned offset;
        clang_getFileLocation(location, nullptr, nullptr, nullptr, &offset);
        return offset_temp_to_original(offset);
    }

    Size ClangParserContext::cursor_end_offset(const CXCursor& cursor) const
    {
        const CXSourceRange extent = clang_getCursorExtent(cursor);
        CXSourceLocation location = clang_getRangeEnd(extent);
        unsigned offset;
        clang_getFileLocation(location, nullptr, nullptr, nullptr, &offset);
        return offset_temp_to_original(offset);
    }

    Size ClangParserContext::cursor_column(const CXCursor& cursor) const
    {
        const CXSourceRange extent = clang_getCursorExtent(cursor);
        CXSourceLocation location = clang_getRangeStart(extent);
        unsigned column;
        clang_getFileLocation(location, nullptr, nullptr, &column, nullptr);
        return column;
    }

} // namespace GodotObjectCompiler

/**************************************************************************/
/* class_handler.cpp                                                      */
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

#include "class_handler.h"

#include "library/tree/syntax/class.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/struct.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    bool ClassHandler::handles_cursor(const CXCursor& p_cursor)
    {
        return cursor_kind_in(
            p_cursor, {CXCursor_ClassDecl, CXCursor_StructDecl, CXCursor_ClassTemplate});
    }

    IClangASTHandler::Step
    ClassHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        if (!clang_isCursorDefinition(p_cursor)) {
            // Cursor is a forward declaration, Skip
            return Step::Over();
        }

        const ClangString name = clang_getCursorSpelling(p_cursor);

        Ref<Body> body;

        if (p_cursor.kind == CXCursor_StructDecl) {
            p_root = p_target->B<Struct>();
        } else {
            p_root = p_target->B<Class>();
        }

        p_root->B<Identifier>(name);

        if (p_cursor.kind == CXCursor_ClassTemplate) {
            p_root->B<TemplateParameters>();
        }

        p_root->B<BaseClasses>();
        p_target = p_root->B<Body>();

        return Step::Into();
    }
} // namespace GodotObjectCompiler::ClangASTHandlers

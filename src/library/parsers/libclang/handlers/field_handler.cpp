/**************************************************************************/
/* field_handler.cpp                                                      */
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

#include "field_handler.h"

#include "library/tree/syntax/field.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler::ClangASTHandlers
{

    bool FieldHandler::handles_cursor(const CXCursor& p_cursor)
    {
        return cursor_kind_in(p_cursor, {CXCursor_FieldDecl, CXCursor_VarDecl});
    }

    IClangASTHandler::Step
    FieldHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        UNUSED(p_root);

        const ClangString name = clang_getCursorDisplayName(p_cursor);
        const ClangString type_name = clang_getTypeSpelling(clang_getCursorType(p_cursor));

        auto type_result = get_cursor_type(p_cursor);
        if (type_result.has_error()) {
            type_result.get_error()->set_handled();
            CLANG_PARS_ERR(type_result.get_error()->message);
        }
        auto type = type_result.get_result();

        auto storage_class = clang_Cursor_getStorageClass(p_cursor);

        p_target = p_target->B<Field>();
        if (storage_class == CX_SC_Static) {
            p_target->B<Static>();
        }

        p_target->add_child(type);
        p_target->B<Identifier>(name);

        if (const String assigned = get_cursor_assigned_value(p_cursor); !assigned.empty()) {
            p_target->B<Literal>(assigned);
            return Step::Over();
        }

        return Step::Into();
    }
} // namespace GodotObjectCompiler::ClangASTHandlers

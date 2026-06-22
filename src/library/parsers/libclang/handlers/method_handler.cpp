/**************************************************************************/
/* method_handler.cpp                                                     */
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
#include "method_handler.h"

#include "library/tree/syntax/function.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler::ClangASTHandlers
{

    bool MethodHandler::handles_cursor(const CXCursor& p_cursor)
    {
        return cursor_kind_in(p_cursor, {CXCursor_CXXMethod, CXCursor_FunctionDecl});
    }

    IClangASTHandler::Step
    MethodHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        const ClangString name = clang_getCursorSpelling(p_cursor);

        const auto type_result = get_cursor_type(p_cursor);
        if (type_result.has_error()) {
            type_result.get_error()->set_handled();
            CLANG_PARS_ERR(type_result.get_error()->message);
        }
        auto type = type_result.get_result();

        p_root = p_target->B<Function>();

        if (p_cursor.kind == CXCursor_CXXMethod) {
            if (clang_CXXMethod_isStatic(p_cursor)) {
                p_root->B<Static>();
            }

            if (clang_CXXMethod_isVirtual(p_cursor)) {
                p_root->B<Virtual>();
            }
        }

        p_root->add_child(type);
        p_root->B<Identifier>(name);

        p_target = p_root->B<Parameters>();

        if (p_cursor.kind == CXCursor_CXXMethod && clang_CXXMethod_isConst(p_cursor)) {
            p_root->B<Const>();
        }

        return Step::Into();
    }
} // namespace GodotObjectCompiler::ClangASTHandlers

/**************************************************************************/
/* base_class_handler.cpp                                                 */
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

#include "base_class_handler.h"

#include "library/tree/syntax/class.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    bool BaseClassHandler::handles_cursor(const CXCursor& p_cursor)
    {
        return p_cursor.kind == CXCursor_CXXBaseSpecifier;
    }

    IClangASTHandler::Step
    BaseClassHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        UNUSED(p_target);

        Ref<Class> _class = p_root->as<Class>();
        PARSER_ERROR_COND(_class == nullptr, "Invalid root for base class handler");

        Ref<BaseClasses> base_classes = _class->find_child<BaseClasses>();
        PARSER_ERROR_COND(base_classes == nullptr, "Could not find base classes in root class.");

        const ClangString name = clang_getCursorDisplayName(p_cursor);
        base_classes->B<Type>()[{B<Identifier>(name)}];
        return Step::Over();
    }
} // namespace GodotObjectCompiler::ClangASTHandlers
/**************************************************************************/
/* template_handler.cpp                                                   */
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

#include "template_handler.h"

namespace GodotObjectCompiler::ClangASTHandlers
{
    bool TemplateHandler::handles_cursor(CXCursor p_cursor)
    {
        return cursor_kind_in(p_cursor, {CXCursor_TemplateTypeParameter});
    }

    IClangASTHandler::Step
    TemplateHandler::handle(const CXCursor& p_cursor, Ref<Context>& p_target, Ref<Context>& p_root)
    {
        switch (p_cursor.kind) {
        case CXCursor_ClassTemplate: {
            p_target->B<TemplateParameters>();
            return Step::Into();
        }
        case CXCursor_TemplateTypeParameter: {
            Ref<TemplateParameters> parameters = p_root->find_child<TemplateParameters>();
            if (parameters == nullptr) {
                return Step::Over();
            }
            PARSER_ERROR_COND(
                parameters == nullptr, "Could not find TemplateParameters child in current root");

            parameters->parameter_count += 1;

            if (!get_cursor_assigned_value(p_cursor).empty()) {
                parameters->optional_parameter_count += 1;
            }
            return Step::Over();
        }
        default:
            PARSER_ERROR("Invalid cursor kind.");
        }
    }
} // namespace GodotObjectCompiler::ClangASTHandlers
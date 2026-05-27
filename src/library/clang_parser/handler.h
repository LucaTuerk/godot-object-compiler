/**************************************************************************/
/* handler.h                                                              */
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

#pragma once

#include "library/core/core.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/parser_error.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler
{
    class ClangParser;
}

namespace GodotObjectCompiler::ClangASTHandlers
{
    class IClangASTHandler
    {
      public:
        class Step
        {
          public:
            enum Type { StepOver, StepInto, Error };

            static Step Into();
            static Step Over();
            Step(const Ref<ParserError>& p_error) : type(Error), error(p_error)
            {
            }

            [[nodiscard]] bool is_into() const;
            [[nodiscard]] bool is_over() const;
            [[nodiscard]] bool is_error() const;
            Ref<ParserError> get_error();

          private:
            Step() = default;

            Type type = StepOver;
            Ref<ParserError> error = nullptr;
        };

        virtual ~IClangASTHandler() = default;
        virtual int get_priority() const;
        virtual bool handles_node(CXCursor p_cursor) = 0;
        virtual Step handle(CXCursor p_cursor, Ref<Context>& p_target, Ref<Context>& p_root) = 0;

      protected:
        bool
        cursor_kind_in(const CXCursor& p_cursor, std::initializer_list<CXCursorKind>&& p_kinds);

        String get_cursor_spelling(const CXCursor& p_cursor);

        Result<Type, ParserError> get_cursor_type(const CXCursor& p_cursor);

        Result<Type, ParserError> get_type_from_spelling(const String& p_spelling);
    };

} // namespace GodotObjectCompiler::ClangASTHandlers

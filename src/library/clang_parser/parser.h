/**************************************************************************/
/* parser.h                                                               */
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
#include "handler.h"
#include "library/parser.h"

namespace GodotObjectCompiler
{
    struct ClangParserContext {
        Ref<Context> root;
        Ref<Context> current;
        CXTranslationUnit unit;
    };

    class ClangParser : public IParser
    {
      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target);

        template <typename T> static bool register_handler(const String& p_name);

        void set_parse_attributes(bool p_parse_attributes);

      private:
        static CXChildVisitResult
        visitor(CXCursor p_cursor, CXCursor p_parent, CXClientData p_data);

        static inline Dictionary<String, Ref<ClangASTHandlers::IClangASTHandler>> handlers;

        bool parse_attributes = true;
    };

    template <typename T> bool ClangParser::register_handler(const String& p_name)
    {
        auto [itr, success] = handlers.emplace(p_name, new T());
        return success;
    }

    class ClangString
    {
      public:
        ClangString(CXString p_string);
        ClangString(const ClangString& p_string);
        ~ClangString();

        operator String() const;

      private:
        int* ref_count;
        CXString data{};
    };
} // namespace GodotObjectCompiler

#define CLANG_AST_HANDLER(type)                                                                    \
    static inline bool __handler_registered__ = ClangParser::register_handler<type>(#type)

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
#include "clang-c/Index.h"
#include "handler.h"
#include "library/parser.h"

namespace GodotObjectCompiler
{

    class ClangParser : public IParser
    {
        PARSER(ClangParser);
        CAPABILITIES(SOURCE_PARSER | SUPPORT_MACRO_EXPANSION);

      public:
        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target) override;

        template <typename T> static bool register_handler();

        void config(Config p_config) override;

      private:
        static CXChildVisitResult
        visitor(CXCursor p_cursor, CXCursor p_parent, CXClientData p_data);

        static inline Vector<Ref<ClangASTHandlers::IClangASTHandler>> handlers;

        bool parse_attributes = false;

        Opt<String> current_file;
    };

    template <typename T> bool ClangParser::register_handler()
    {
        handlers.push_back(std::make_shared<T>());
        std::sort(handlers.begin(), handlers.end(), [](auto handler_a, auto p_handler_b) {
            return handler_a->get_priority() > p_handler_b->get_priority();
        });
        return true;
    }

    template <typename T, auto Dispose> class ClangLocalRAII
    {
      public:
        ClangLocalRAII(T&& p_data);

        ClangLocalRAII(const ClangLocalRAII& p_other) = delete;

        ClangLocalRAII& operator=(const ClangLocalRAII& p_other) = delete;

        ~ClangLocalRAII();

        operator T&();
        T _data;
    };

    class ClangString : public ClangLocalRAII<CXString, &clang_disposeString>
    {
      public:
        // clang-format off
        ClangString(CXString&& data) : ClangLocalRAII(std::move(data)) {};
        // clang-format on
        operator String() const;
    };

    template <typename T, auto Dispose>
    ClangLocalRAII<T, Dispose>::ClangLocalRAII(T&& p_data) : _data(std::move(p_data))
    {
    }

    template <typename T, auto Dispose> ClangLocalRAII<T, Dispose>::~ClangLocalRAII()
    {
        Dispose(_data);
    }

    template <typename T, auto Dispose> ClangLocalRAII<T, Dispose>::operator T&()
    {
        return _data;
    }

    using ClangTranslationUnit = ClangLocalRAII<CXTranslationUnit, &clang_disposeTranslationUnit>;
    using ClangDiagnostic = ClangLocalRAII<CXDiagnostic, &clang_disposeDiagnostic>;
    using ClangEvalResult = ClangLocalRAII<CXEvalResult, &clang_EvalResult_dispose>;

} // namespace GodotObjectCompiler

#define CLANG_AST_HANDLER(type)                                                                    \
    static inline bool __handler_registered__ = ClangParser::register_handler<type>()

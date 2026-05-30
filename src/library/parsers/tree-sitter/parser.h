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

#include "library/core/core.h"
#include "library/parser.h"
#include "library/tree/syntax/node.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler
{
    class ITreeSitterNodeHandler;
    class TreeSitterNode;
    struct ParserContext;

    class TreeSitterParser : public IParser
    {
        PARSER(TreeSitterParser);

      public:
        ~TreeSitterParser() override = default;

        Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target) override;

        Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

        int get_capabilities() override;

        template <typename T> static bool register_handler(const String& p_name);

        void config(Config p_config) override;

      private:
        bool parse_attributes = true;
        String input_path;
        static inline HashSet<String> _registered_handlers;
        static inline Vector<Ref<ITreeSitterNodeHandler>> _handlers;
    };

    template <typename T> bool TreeSitterParser::register_handler(const String& p_name)
    {
        if (_registered_handlers.find(p_name) != _registered_handlers.end()) {
            return false;
        }
        _registered_handlers.insert(p_name);
        _handlers.push_back(make_ref<T>());
        return true;
    }

} // namespace GodotObjectCompiler

#define HANDLER_ERROR(...)                                                                         \
    node_new<ParserError>(ERROR, format(__VA_ARGS__));                                             \
    return ParserStep::StepOver();

#define HANDLER_ERROR_COND(condition, ...)                                                         \
    if ((condition)) {                                                                             \
        HANDLER_ERROR(__VA_ARGS__)                                                                 \
    }

/**************************************************************************/
/* tree_sitter_node.h                                                     */
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

#include "library/core/string_utilities.h"
#include "library/tree/syntax/context.h"
#include "tree_sitter/api.h"

namespace GodotObjectCompiler
{

    struct ParserContext;

    class TreeSitterNode : public Context
    {
        NODE_TYPE(TreeSitterNode);

      public:
        struct Point {
            Size row = 0;
            Size column = 0;

            Point() = default;
            Point(const TSPoint& point);
        };

        TreeSitterNode(const TSNode& node, ParserContext* parser_context);

        String to_string() const override;

        bool type_in(std::initializer_list<String>&& p_type_names) const;

        bool is_handled() const;

        void set_handled(bool p_handled = true);

        ParserContext* context;
        String type;
        Size start_byte;
        Size end_byte;
        Point start_point;
        Point end_point;
        bool handled = false;

        bool copy_to(const Ref<Node>& p_other) const override;

        LAZY(TreeSitterNode, String, content);
    };

    inline Predicate<TreeSitterNode> type_is(const String& p_type)
    {
        return [p_type](Ref<TreeSitterNode> node) { return node->type == p_type; };
    }

    inline Predicate<TreeSitterNode> type_suffix(const String& p_suffix)
    {
        return [p_suffix](Ref<TreeSitterNode> node) { return string_suffix(node->type, p_suffix); };
    }

    inline Predicate<TreeSitterNode> type_contains(const String& p_search)
    {
        return
            [p_search](Ref<TreeSitterNode> node) { return string_contains(node->type, p_search); };
    }

    inline Predicate<TreeSitterNode> type_in(std::initializer_list<String>&& p_types)
    {
        return [p_types](Ref<TreeSitterNode> node) {
            return std::find(p_types.begin(), p_types.end(), node->type) != p_types.end();
        };
    }

} // namespace GodotObjectCompiler

/**************************************************************************/
/* parser_context.cpp                                                     */
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
#include "library/parser/parser_context.h"

#include "library/core/file_system_utilities.h"
#include "library/parser/parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/all.h"

namespace GodotObjectCompiler {

  bool ParserContext::is_valid() const { return current_src && !current_src->empty(); }

  ParserContext::ParserContext(const String& input) {
    buffer = input;
    current_target = ExecutionContext::instance()->get_node_db()->create<Namespace>();

    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree* tree = ts_parser_parse_string(parser, nullptr, buffer.c_str(), buffer.length());
    TSNode node = ts_tree_root_node(tree);

    _invalid = ts_node_is_null(node);
    current_src = _invalid ? nullptr : create_node(node);
    src_root = current_src;

    ts_tree_delete(tree);
    ts_parser_delete(parser);
  }

  ParserContext ParserContext::from_path(const String& p_path) {
    ParserContext result{read_file(p_path)};
    result.file_path = p_path;
    return result;
  }

  Ref<TreeSitterNode> ParserContext::create_tree(TSTree* p_tree) {
    TSNode ts_root = ts_tree_root_node(p_tree);
    if (ts_node_is_null(ts_root)) {
      return nullptr;
    }

    return create_node(ts_root);
  }

  Ref<TreeSitterNode> ParserContext::create_node(TSNode p_ts_node) {
    if (ts_node_is_null(p_ts_node) || String(ts_node_type(p_ts_node)) == "comment") {
      return nullptr;
    }

    Ref<TreeSitterNode> node = node_new<TreeSitterNode>(p_ts_node, this);
    for (uint32_t i = 0; i < ts_node_child_count(p_ts_node); ++i) {
      if (auto child = create_node(ts_node_child(p_ts_node, i))) {
        node->add_child(child);
      }
    }

    return node;
  }

}

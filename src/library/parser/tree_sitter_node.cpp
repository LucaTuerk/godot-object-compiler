/**************************************************************************/
/* tree_sitter_node.cpp                                                   */
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

#include "tree_sitter_node.h"

#include "parser_context.h"

namespace GodotObjectCompiler
{

TreeSitterNode::Point::Point(const TSPoint& point)
{
  row = point.row;
  column = point.column;
}

TreeSitterNode::TreeSitterNode(
    const TSNode& node, ParserContext* parser_context)
{
  context = parser_context;
  type = ts_node_type(node);
  start_byte = ts_node_start_byte(node);
  end_byte = ts_node_end_byte(node);
  start_point = ts_node_start_point(node);
  end_point = ts_node_end_point(node);
}

String TreeSitterNode::to_string() const { return type; }

bool TreeSitterNode::type_in(std::initializer_list<String>&& p_type_names) const
{
  return std::find(p_type_names.begin(), p_type_names.end(), type) !=
         p_type_names.end();
}

bool TreeSitterNode::is_handled() const { return handled; }

void TreeSitterNode::set_handled(bool p_handled) { this->handled = p_handled; }

bool TreeSitterNode::copy_to(const Ref<Node>& p_other) const
{
  COPY_GUARD(TreeSitterNode, Context)

  target->context = context;
  target->type = type;
  target->start_byte = start_byte;
  target->end_byte = end_byte;
  target->start_point = start_point;
  target->end_point = end_point;
  return true;
}

String TreeSitterNode::_content_lazy_get() const
{
  return context->buffer.substr(start_byte, end_byte - start_byte);
}

} // namespace GodotObjectCompiler
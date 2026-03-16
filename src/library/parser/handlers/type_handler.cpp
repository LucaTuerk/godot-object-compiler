/**************************************************************************/
/* type_handler.cpp                                                       */
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

#include "type_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/struct.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

auto TypeHandler::handles_node(const Ref<TreeSitterNode>& p_current_src)
    -> bool {
  return p_current_src->type_in({"primitive_type", "qualified_identifier",
                                 "type_identifier", "trailing_return_type",
                                 "template_type"});
}

ParserStep TypeHandler::handle(const Ref<TreeSitterNode>& p_current_src,
                               Ref<Context>& r_current_target) {
  if ((r_current_target->is<Class>() || r_current_target->is<Struct>() ||
       r_current_target->is<Enum>()) &&
      (r_current_target->find_child<Identifier>() == nullptr)) {
    r_current_target->create_child<Identifier>(p_current_src->content());
    return ParserStep::StepOver();
  }

  if (p_current_src->type == "template_type") {
    r_current_target = r_current_target->create_child<Type>();
    return ParserStep::Undecided();
  }

  Ref<Type> type_node;
  if (r_current_target->is<Type>()) {
    type_node = r_current_target->as<Type>();
  } else {
    if (Ref<PlaceholderType> placeholder =
            r_current_target->find_child<PlaceholderType>()) {
      type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
      r_current_target->replace_child(placeholder, type_node);
    } else {
      type_node = r_current_target->create_child<Type>();
    }
  }

  Ref<Node> last = type_node->get_previous_sibling();
  bool last_is_qualifier = last && last->is<TypeQualifier>();

  if (last_is_qualifier) {
    last->reparent(type_node);
  }

  type_node->create_child<Identifier>(p_current_src->content());

  if (p_current_src->type == "qualified_identifier") {
    return ParserStep::StepOver();
  } else {
    return ParserStep::Undecided();
  }
}

}  // namespace GodotObjectCompiler

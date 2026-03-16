/**************************************************************************/
/* list_handler.cpp                                                       */
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
#include "list_handler.h"

#include "library/parser/node_handler.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

bool ListHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
  // TODO: Should the be string_suffix?
  return string_contains(p_current_src->type, "list");
}

ParserStep ListHandler::handle(const Ref<TreeSitterNode>& p_current_src,
                               Ref<Context>& r_current_target) {
  if (r_current_target->is<Attribute>()) {
    // Parameters are handled by attributes parser.
    return ParserStep::StepOver();
  }

  if (p_current_src->type_in({"declaration_list", "field_declaration_list"})) {
    r_current_target = r_current_target->create_child<Body>();
  } else if (p_current_src->type == "parameter_list") {
    r_current_target = r_current_target->create_child<Parameters>();
  } else if (p_current_src->type == "template_argument_list") {
    r_current_target = r_current_target->create_child<TemplateArguments>();
  } else if (p_current_src->type == "template_parameter_list") {
    r_current_target = r_current_target->create_child<TemplateParameters>();
  } else if (p_current_src->type == "initializer_list") {
    r_current_target->create_child<Literal>(p_current_src->content());
    return ParserStep::StepOver();
  } else {
    r_current_target = r_current_target->create_child<Context>();
  }

  return ParserStep::StepInto();
}

}  // namespace GodotObjectCompiler

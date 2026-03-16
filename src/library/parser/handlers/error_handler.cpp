/**************************************************************************/
/* error_handler.cpp                                                      */
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
#include "error_handler.h"

#include "library/core/string_writer.h"
#include "library/parser/parser_context.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/node.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

bool ErrorHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
  return p_current_src->type == "ERROR";
}

ParserStep ErrorHandler::handle(const Ref<TreeSitterNode>& p_current_src,
                                Ref<Context>& r_current_target) {
  Vector<Ref<TreeSitterNode>> children =
      p_current_src->find_children<TreeSitterNode>();

  if (children.size() >= 2 && children[0]->type == "#define" &&
      children[1]->type == "identifier") {
    Ref<Define> define =
        r_current_target->build_child<Define>().with_child<Identifier>(
            children[1]->content());
    if (children.size() >= 3 && children[2]->type == "preproc_params") {
      define->build_child<Parameters>();
    }
    return ParserStep::StepOver();
  }

  if (std::all_of(children.begin(), children.end(),
                  type_in({"function_declarator", "field_declarator"}))) {
    return ParserStep::StepOver();
  }

  r_current_target->create_child<ParserError>(
      ERROR, "TreeSitterParser", "", p_current_src->context->file_path,
      p_current_src->context->buffer, p_current_src->start_point.row + 1,
      p_current_src->start_point.column + 1);

  return ParserStep::StepOver();
}

}  // namespace GodotObjectCompiler
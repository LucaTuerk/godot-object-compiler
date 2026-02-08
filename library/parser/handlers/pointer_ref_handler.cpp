/**************************************************************************/
/* pointer_ref_handler.cpp                                                */
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
#include "pointer_ref_handler.h"

#include "library/core/helpers.h"
#include "library/parser/node_handler.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  bool PointerRefHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return string_only_contains(p_current_src->type, '&') || string_only_contains(p_current_src->type, '*');
  }

  ParserStep PointerRefHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    enum Option { REFERENCE, POINTER };

    Ref<Context> target_node = r_current_target->find_child<Type>();
    if (!target_node) {
      target_node = r_current_target;
    }

    Option option = string_only_contains(p_current_src->type, '&') ? REFERENCE : POINTER;
    for (Size i = 0; i < p_current_src->type.length(); ++i) {
      switch (option) {
        case REFERENCE:
          target_node->create_child<Reference>();
          break;
        case POINTER:
          target_node->create_child<Pointer>();
          break;
      }
    }

    return ParserStep::Undecided();
  }

}  // namespace GodotObjectCompiler
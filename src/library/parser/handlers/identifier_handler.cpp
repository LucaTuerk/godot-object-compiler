/**************************************************************************/
/* identifier_handler.cpp                                                 */
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
#include "identifier_handler.h"

#include "library/attribute_db.h"
#include "library/parser/attribute_argument_parser.h"
#include "library/parser/parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler {

  bool IdentifierHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return string_contains(p_current_src->type, "identifier") &&
        !(p_current_src->type == "qualified_identifier" || p_current_src->type == "type_identifier");
  }

  ParserStep IdentifierHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    Ref<Context> target = r_current_target;

    if (r_current_target->is<Field>() && r_current_target->empty()) {
      target = r_current_target->create_child<Field>();
    }

    target->create_child<Identifier>(p_current_src->content());

    if (AttributeDB::instance()->is_known_macro(p_current_src->content())) {
      return handle_known_attribute(p_current_src, r_current_target, p_current_src->content());
    }

    return ParserStep::StepOver();
  }

  ParserStep IdentifierHandler::handle_known_attribute(
      const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target, const String& macro) {
    if (Ref<Context> parent = current_target->get_parent(); parent) {
      Ref<Attribute> attribute = AttributeDB::instance()->create_for_macro(macro);
      HANDLER_ERROR_COND(!attribute, "Failed to create attribute for known macro %s", macro.c_str());

      parent->replace_child(current_target, attribute, true);
      current_target = attribute;

      Ref<TreeSitterNode> ancestor =
          current_src->find_ancestor(DIRECT_PARENTS, type_in({"declaration", "expression_statement"}));

      HANDLER_ERROR_COND(!ancestor, "Failed to create attribute for known macro %s", macro.c_str());
      attribute->start = ancestor->start_byte;
      attribute->end = ancestor->end_byte;
      attribute->line = ancestor->start_point.row + 1;

      Ref<IAttributeArgumentParser> argument_parser = attribute->get_argument_parser();
      if (argument_parser) {
        auto itr = current_src->context->stripped_parameters.find(current_src->start_byte);
        if (itr != current_src->context->stripped_parameters.end()) {
          Ref<ParserError> error =
              argument_parser->parse_attribute_arguments(itr->second, attribute->create_child<Arguments>());

          if (error != ParserError::OK) {
            attribute->remove_all_children();
            attribute->add_child(node_new<ParserError>(error->error_level, "TreeSitterParser",
                format("Failed to parse attribute arguments.\n%s", error->message.c_str()),
                current_src->context->file_path, current_src->context->original_buffer, attribute->line,
                ancestor->start_point.column + 1));
            error->set_handled();
          }
        }
      }

      return ParserStep::GoTo(ancestor);
    }
    return ParserStep::StepOver();
  }

}

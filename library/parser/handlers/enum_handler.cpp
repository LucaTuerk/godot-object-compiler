//
// Created by luca on 20.01.26.
//

#include "enum_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

  bool EnumHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"enum_specifier", "enumerator_list", "enumerator"});
  }

  ParserStep EnumHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->type == "enum_specifier") {
      current_target = current_target->create_child<Enum>();
    }
    if (current_src->type == "enumerator_list") {
      current_target = current_target->create_child<EnumValues>();
    }
    if (current_src->type == "enumerator") {
      current_target = current_target->create_child<EnumValue>();
      if (current_src
              ->find_children<TreeSitterNode>(
                  true, TreeSitterNodePredicates::types({"number_literal", "binary_expression"}))
              .empty()) {
        if (const Ref<TreeSitterNode> identifier =
                current_src->find_child(0, TreeSitterNodePredicates::type("identifier"))) {
          current_target->create_child<Identifier>(identifier->content());
          identifier->set_handled();
        }

        if (const Ref<EnumValue> prev = current_target->find_previous_sibling<EnumValue>()) {
          if (const Ref<Literal> literal = prev->find_child<Literal>()) {
            const int val = string_to_int(literal->content);
            current_target->create_child<Literal>(std::to_string(val + 1));
          }
        } else {
          current_target->create_child<Literal>(std::to_string(0));
        }
      }
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
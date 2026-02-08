//
// Created by luca on 20.01.26.
//

#include "enum_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

  bool EnumHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type_in({"enum_specifier", "enumerator_list", "enumerator"});
  }

  ParserStep EnumHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->type == "enum_specifier") {
      r_current_target = r_current_target->create_child<Enum>();
    }
    if (p_current_src->type == "enumerator_list") {
      r_current_target = r_current_target->create_child<EnumValues>();
    }
    if (p_current_src->type == "enumerator") {
      r_current_target = r_current_target->create_child<EnumValue>();
      if (p_current_src->find_children<TreeSitterNode>(true, type_in({"number_literal", "binary_expression"}))
              .empty()) {
        if (const Ref<TreeSitterNode> identifier = p_current_src->find_child(0, type_is("identifier"))) {
          r_current_target->create_child<Identifier>(identifier->content());
          identifier->set_handled();
        }

        if (const Ref<EnumValue> prev = r_current_target->find_previous_sibling<EnumValue>()) {
          if (const Ref<Literal> literal = prev->find_child<Literal>()) {
            const int val = string_to_int(literal->content);
            r_current_target->create_child<Literal>(std::to_string(val + 1));
          }
        } else {
          r_current_target->create_child<Literal>(std::to_string(0));
        }
      }
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
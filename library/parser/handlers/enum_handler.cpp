//
// Created by luca on 20.01.26.
//

#include "enum_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/enum.h"

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
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
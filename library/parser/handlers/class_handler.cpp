//
// Created by luca on 17.01.26.
//

#include "class_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool ClassHandler::handles_node(TSNode& node, const String& type) {
  //   return String(ts_node_type(node)) == "class_specifier";
  // }
  //
  // NextStep ClassHandler::handle(ParserContext& context) {
  //   bool success = true;
  //   context.get_descendant_by_type("field_declaration_list", success);
  //
  //   if (!success) {
  //     // This is probably a forward declare. Skip.
  //     return STEP_OVER;
  //   }
  //   context.current_node = context.current_node->build_child<Class>();
  //   return STEP_INTO;
  // }

  bool ClassHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "class_specifier";
  }

  ParserStep ClassHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> found = current_src->find_descendant<TreeSitterNode>(BFS, type_is("field_declaration_list"));
    if (!found) {
      return ParserStep::StepOver();
    }

    current_target = current_target->build_child<Class>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
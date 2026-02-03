//
// Created by luca on 18.01.26.
//

#include "type_qualifier_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool TypeQualifierHandler::handles_node(TSNode& node, const String& type) { return type == "type_qualifier"; }
  //
  // NextStep TypeQualifierHandler::handle(ParserContext& context) {
  //   String content = context.copy_node_content(context.node);
  //
  //   if (content == "const") {
  //     context.current_node->create_child<Const>();
  //   } else if (content == "mutable") {
  //     context.current_node->create_child<Mutable>();
  //   } else if (content == "volatile") {
  //     context.current_node->create_child<Volatile>();
  //   }
  //
  //   return STEP_INTO;
  // }
  //
  // bool TypeQualifierHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
  //   return current_src->type == "type_qualifier";
  // }

  ParserStep TypeQualifierHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->content() == "const") {
      current_target->create_child<Const>();
    } else if (current_src->content() == "mutable") {
      current_target->create_child<Mutable>();
    } else if (current_src->content() == "volatile") {
      current_target->create_child<Volatile>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
//
// Created by luca on 20.01.26.
//

#include "literal_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool LiteralHandler::handles_node(TSNode& node, const String& type) {
  //   return string_suffix(type, "literal") || type == "null";
  // }
  //
  // NextStep LiteralHandler::handle(ParserContext& context) {
  //   context.current_node->create_child<Literal>(context.copy_node_content(context.node));
  //   return STEP_OVER;
  // }

  bool LiteralHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_suffix(current_src->type, "literal") || current_src->type == "null";
  }

  ParserStep LiteralHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target->create_child<Literal>(current_src->content());
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
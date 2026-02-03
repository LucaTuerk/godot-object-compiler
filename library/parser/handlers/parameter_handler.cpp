//
// Created by luca on 18.01.26.
//

#include "parameter_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool ParameterHandler::handles_node(TSNode& node, const String& type) {
  //   return type == "parameter_declaration" || type == "optional_parameter_declaration";
  // }
  //
  // NextStep ParameterHandler::handle(ParserContext& context) {
  //   context.current_node = context.current_node->create_child<Parameter>();
  //   return STEP_INTO;
  // }

  bool ParameterHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"parameter_declaration", "optional_parameter_declaration"});
  }

  ParserStep ParameterHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target = current_target->create_child<Parameter>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler

#include "operator_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool OperatorHandler::handles_node(TSNode& node, const String& type) { return type == "operator_cast"; }
  //
  // NextStep OperatorHandler::handle(ParserContext& context) {
  //   if (context.current_node->is<Field>()) {
  //     Ref<Context> parent = context.current_node->get_parent();
  //     if (parent) {
  //       auto operator_node = node_new<Operator>();
  //       parent->replace_child(context.current_node, operator_node, true);
  //       context.current_node = operator_node;
  //     }
  //   }
  //   return STEP_INTO;
  // }

  bool OperatorHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "operator_cast";
  }

  ParserStep OperatorHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_target->is<Field>()) {
      Ref<Context> parent = current_target->get_parent();
      if (parent) {
        Ref<Operator> operator_node = node_new<Operator>();
        parent->replace_child(current_target, operator_node, true);
        current_target = operator_node;
      }
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
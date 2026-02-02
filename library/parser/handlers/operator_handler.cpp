
#include "operator_handler.h"

namespace GodotObjectCompiler {

  bool OperatorHandler::handles_node(TSNode& node, const String& type) { return type == "operator_cast"; }

  NextStep OperatorHandler::handle(ParserContext& context) {
    if (context.current_node->is<Field>()) {
      Ref<Context> parent = context.current_node->get_parent();
      if (parent) {
        auto operator_node = node_new<Operator>();
        parent->replace_child(context.current_node, operator_node, true);
        context.current_node = operator_node;
      }
    }
    return STEP_INTO;
  }

}  // namespace GodotObjectCompiler
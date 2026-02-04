
#include "operator_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/misc_keywords.h"

namespace GodotObjectCompiler {

  bool OperatorHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "operator_cast";
  }

  ParserStep OperatorHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
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
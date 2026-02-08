
#include "operator_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/misc_keywords.h"

namespace GodotObjectCompiler {

  bool OperatorHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "operator_cast";
  }

  ParserStep OperatorHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (r_current_target->is<Field>()) {
      Ref<Context> parent = r_current_target->get_parent();
      if (parent) {
        Ref<Operator> operator_node = node_new<Operator>();
        parent->replace_child(r_current_target, operator_node, true);
        r_current_target = operator_node;
      }
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
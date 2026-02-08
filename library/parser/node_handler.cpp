
#include "node_handler.h"

namespace GodotObjectCompiler {

  ParserStep ParserStep::Undecided() { return {TYPE_UNDECIDED, nullptr}; }

  ParserStep ParserStep::StepInto() { return {TYPE_STEP_INTO, nullptr}; }

  ParserStep ParserStep::StepOver() { return {TYPE_STEP_OVER, nullptr}; }

  ParserStep ParserStep::StepOut() { return {TYPE_STEP_OUT, nullptr}; }

  ParserStep ParserStep::GoTo(const Ref<TreeSitterNode>& target) { return {TYPE_UNDECIDED, target}; }

  bool ParserStep::is_undecided() const { return type == TYPE_UNDECIDED; }

  bool ParserStep::is_step_into() const { return type == TYPE_STEP_INTO; }

  bool ParserStep::is_step_over() const { return type == TYPE_STEP_OVER; }

  bool ParserStep::is_step_out() const { return type == TYPE_STEP_OUT; }

  bool ParserStep::is_go_to(Ref<TreeSitterNode>& p_target) const {
    const bool result = type == TYPE_GO_TO;
    if (result) {
      p_target = goto_target;
    }
    return result;
  }

}  // namespace GodotObjectCompiler
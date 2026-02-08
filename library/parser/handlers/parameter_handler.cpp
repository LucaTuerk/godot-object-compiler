#include "parameter_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool ParameterHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type_in({"parameter_declaration", "optional_parameter_declaration"});
  }

  ParserStep ParameterHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    r_current_target = r_current_target->create_child<Parameter>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
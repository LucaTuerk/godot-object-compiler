#include "parameter_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool ParameterHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"parameter_declaration", "optional_parameter_declaration"});
  }

  ParserStep ParameterHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target = current_target->create_child<Parameter>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
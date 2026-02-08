
#include "library/parser/handlers/namespace_handler.h"

#include "library/core/core.h"
#include "library/parser/node_handler.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  bool NamespaceHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "namespace_definition";
  }

  ParserStep NamespaceHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    r_current_target = r_current_target->build_child<Namespace>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler

#include "library/parser/handlers/namespace_handler.h"

#include "library/core/core.h"
#include "library/parser/node_handler.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool NamespaceHandler::handles_node(TSNode& node, const String& type) {
  //   return String(ts_node_type(node)) == "namespace_definition";
  // }
  //
  // NextStep NamespaceHandler::handle(ParserContext& context) {
  //   context.current_node = context.current_node->build_child<Namespace>();
  //   return STEP_INTO;
  // }

  bool NamespaceHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "namespace_definition";
  }

  ParserStep NamespaceHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target = current_target->build_child<Namespace>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
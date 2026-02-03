#include "include_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool IncludeHandler::handles_node(TSNode& node, const String& type) { return type == "preproc_include"; }
  //
  // NextStep IncludeHandler::handle(ParserContext& context) {
  //   bool success = false, system_success = false;
  //   TSNode content = context.get_descendant_by_type("string_content", success);
  //   if (!success) {
  //     content = context.get_descendant_by_type("system_lib_string", system_success);
  //   }
  //
  //   if (success) {
  //     Ref<Include> include = context.current_node->create_child<Include>(context.copy_node_content(content), false);
  //   } else if (system_success) {
  //     Ref<Include> include = context.current_node->create_child<Include>(context.copy_node_content(content), true);
  //   } else {
  //     print_err("Could not find string_literal node");
  //   }
  //
  //   return STEP_OVER;
  // }

  bool IncludeHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "preproc_include";
  }

  ParserStep IncludeHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> path = current_src->find_descendant(BFS, type_in({"string_content", "system_lib_string"}));
    HANDLER_ERROR_COND_V2(!path, "Failed to get include path.")

    if (path->type == "preproc_include") {
      current_target->create_child<Include>(path->content(), false);
    } else {
      current_target->create_child<Include>(path->content(), true);
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
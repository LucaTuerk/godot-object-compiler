#include "include_handler.h"

#include "library/parser/parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/include.h"

namespace GodotObjectCompiler {

  bool IncludeHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "preproc_include";
  }

  ParserStep IncludeHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> path = current_src->find_descendant(BFS, type_in({"string_content", "system_lib_string"}));
    HANDLER_ERROR_COND(!path, "Failed to get include path.")

    if (path->type == "preproc_include") {
      current_target->create_child<Include>(path->content(), false);
    } else {
      current_target->create_child<Include>(path->content(), true);
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
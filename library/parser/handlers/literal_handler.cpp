
#include "literal_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

  bool LiteralHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_suffix(current_src->type, "literal") || current_src->type == "null";
  }

  ParserStep LiteralHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target->create_child<Literal>(current_src->content());
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler

#include "literal_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

  bool LiteralHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return string_suffix(p_current_src->type, "literal") || p_current_src->type == "null";
  }

  ParserStep LiteralHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    r_current_target->create_child<Literal>(p_current_src->content());
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
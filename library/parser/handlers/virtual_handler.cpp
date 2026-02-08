#include "virtual_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  bool VirtualHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return string_contains(p_current_src->type, "virtual") || string_contains(p_current_src->type, "virtual_specifier");
  }

  ParserStep VirtualHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->content() == "virtual") {
      r_current_target->create_child<Virtual>();
    } else if (p_current_src->content() == "override") {
      r_current_target->create_child<Override>();
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
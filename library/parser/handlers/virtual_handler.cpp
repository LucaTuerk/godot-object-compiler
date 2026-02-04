#include "virtual_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  bool VirtualHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_contains(current_src->type, "virtual") || string_contains(current_src->type, "virtual_specifier");
  }

  ParserStep VirtualHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->content() == "virtual") {
      current_target->create_child<Virtual>();
    } else if (current_src->content() == "override") {
      current_target->create_child<Override>();
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
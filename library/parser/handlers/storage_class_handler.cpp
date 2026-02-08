#include "storage_class_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  bool StorageClassHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return string_contains(p_current_src->type, "storage_class_specifier");
  }

  ParserStep StorageClassHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->content() == "static") {
      r_current_target->create_child<Static>();
    } else if (p_current_src->content() == "inline") {
      r_current_target->create_child<Inline>();
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
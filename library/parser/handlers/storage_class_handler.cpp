#include "storage_class_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  bool StorageClassHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_contains(current_src->type, "storage_class_specifier");
  }

  ParserStep StorageClassHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->content() == "static") {
      current_target->create_child<Static>();
    } else if (current_src->content() == "inline") {
      current_target->create_child<Inline>();
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
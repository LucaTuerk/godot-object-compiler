//
// Created by luca on 17.01.26.
//

#include "storage_class_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  bool StorageClassHandler::handles_node(TSNode& node, const String& type) {
    return string_contains(type, "storage_class_specifier");
  }

  NextStep StorageClassHandler::handle(ParserContext& context) {
    auto content = context.copy_node_content(context.node);
    if (content == "static") {
      context.current_node->create_child<Static>();
    }

    if (content == "inline") {
      context.current_node->create_child<Inline>();
    }

    return STEP_OVER;
  }

  bool StorageClassHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_contains(current_src->type, "storage_class_specifier");
  }

  ParserStep StorageClassHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->content() == "static") {
      current_target->create_child<Static>();
    } else if (current_src->content() == "inline") {
      current_target->create_child<Inline>();
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
//
// Created by luca on 17.01.26.
//

#include "storage_class_handler.h"

#include "library/core/helpers.h"

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

}  // namespace GodotObjectCompiler
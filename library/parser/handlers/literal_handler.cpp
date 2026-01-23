//
// Created by luca on 20.01.26.
//

#include "literal_handler.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  bool LiteralHandler::handles_node(TSNode& node, const String& type) {
    return string_suffix(type, "literal") || type == "null";
  }

  NextStep LiteralHandler::handle(ParserContext& context) {
    context.current_node->create_child<Literal>(context.copy_node_content(context.node));
    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
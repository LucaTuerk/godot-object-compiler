//
// Created by luca on 20.01.26.
//

#include "error_handler.h"

#include "library/tree/syntax/node.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  bool ErrorHandler::handles_node(TSNode& node, const String& type) { return type == "ERROR"; }

  NextStep ErrorHandler::handle(ParserContext& context) {
    context.current_node->create_child<ParserError>(context.copy_node_content(context.node));
    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
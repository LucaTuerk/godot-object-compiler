//
// Created by luca on 18.01.26.
//

#include "parameter_handler.h"

namespace GodotObjectCompiler {

  bool ParameterHandler::handles_node(TSNode& node, const String& type) {
    return type == "parameter_declaration";
  }

  NextStep ParameterHandler::handle(ParserContext& context) {
    context.current_node = context.current_node->create_child<Parameter>();
    return STEP_INTO;
  }

}  // namespace GodotObjectCompiler
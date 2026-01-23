//
// Created by luca on 20.01.26.
//

#include "enum_handler.h"

namespace GodotObjectCompiler {

  bool EnumHandler::handles_node(TSNode& node, const String& type) {
    return type == "enum_specifier" || type == "enumerator_list" || type == "enumerator";
  }

  NextStep EnumHandler::handle(ParserContext& context) {
    String type = ts_node_type(context.node);

    if (type == "enum_specifier") {
      context.current_node = context.current_node->create_child<Enum>();
    }
    if (type == "enumerator_list") {
      context.current_node = context.current_node->create_child<EnumValues>();
    }
    if (type == "enumerator") {
      context.current_node = context.current_node->create_child<EnumValue>();
    }

    return STEP_INTO;
  }
}  // namespace GodotObjectCompiler
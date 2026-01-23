//
// Created by luca on 17.01.26.
//

#include "class_handler.h"

namespace GodotObjectCompiler {

  bool ClassHandler::handles_node(TSNode& node, const String& type) {
    return String(ts_node_type(node)) == "class_specifier";
  }

  NextStep ClassHandler::handle(ParserContext& context) {
    bool success = true;
    context.get_descendant_by_type("field_declaration_list", success);

    if (!success) {
      // This is probably a forward declare. Skip.
      return STEP_OVER;
    }
    context.current_node = context.create_class();
    return STEP_INTO;
  }

}  // namespace GodotObjectCompiler
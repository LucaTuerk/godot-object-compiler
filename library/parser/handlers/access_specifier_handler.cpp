//
// Created by luca on 17.01.26.
//

#include "access_specifier_handler.h"

namespace GodotObjectCompiler {

  bool AccessSpecifierHandler::handles_node(TSNode& node, const String& type) {
    return String(ts_node_type(node)) == "access_specifier";
  }

  NextStep AccessSpecifierHandler::handle(ParserContext& context) {
    const String value = context.copy_node_content(context.node);
    AccessSpecifier* as = context.current_node->create_child<AccessSpecifier>();

    if (value == "public") {
      as->type = AccessSpecifier::PUBLIC;
    } else if (value == "protected") {
      as->type = AccessSpecifier::PROTECTED;
    } else if (value == "private") {
      as->type = AccessSpecifier::PRIVATE;
    }

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler

#include "sized_specifier_handler.h"

namespace GodotObjectCompiler {

  bool SizedSpecifierHandler::handles_node(TSNode& node, const String& type) {
    return type == "sized_type_specifier" || type == "long" || type == "short" || type == "unsigned" ||
           type == "signed";
  }

  NextStep SizedSpecifierHandler::handle(ParserContext& context) {
    String type = ts_node_type(context.node);

    if (type == "unsigned") {
      context.current_node->create_child<Unsigned>();
    } else if (type == "signed") {
      context.current_node->create_child<Signed>();
    } else if (type == "long") {
      context.current_node->create_child<Long>();
    } else if (type == "short") {
      context.current_node->create_child<Short>();
    } else if (type == "sized_type_specifier") {
      context.current_node = context.current_node->create_child<Type>();
      return STEP_INTO;
    }

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
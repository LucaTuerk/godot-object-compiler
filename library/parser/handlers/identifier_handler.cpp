//
// Created by luca on 18.01.26.
//

#include "identifier_handler.h"

namespace GodotObjectCompiler {

  bool IdentifierHandler::handles_node(TSNode& node, const String& type) {
    return string_contains(type, "identifier") &&
           !(type == "qualified_identifier" || type == "type_identifier");
  }

  NextStep IdentifierHandler::handle(ParserContext& context) {
    Context* target = context.current_node;

    if (dynamic_cast<Field*>(context.current_node) &&
        context.current_node->get_child_count() == 0) {
      target = context.current_node->create_child<Type>();
    }

    Identifier* identifier = target->create_child<Identifier>();
    identifier->name = context.copy_node_content(context.node);
    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
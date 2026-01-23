//
// Created by luca on 18.01.26.
//

#include "type_handler.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  bool TypeHandler::handles_node(TSNode& node, const String& type) {
    return string_suffix(type, "type") || type == "qualified_identifier" || type == "type_identifier" ||
           type == "trailing_return_type";
  }

  NextStep TypeHandler::handle(ParserContext& context) {
    if (context.current_node->is<Class>() || context.current_node->is<Struct>() || context.current_node->is<Enum>()) {
      Identifier* class_name = context.current_node->create_child<Identifier>();
      class_name->name = context.copy_node_content(context.node);
      return STEP_OVER;
    }

    String type = ts_node_type(context.node);
    Node* last = context.current_node->get_child(-1);
    bool last_is_qualifier = last && last->is<TypeQualifier>();

    Type* type_node;
    if (context.current_node->is<Type>()) {
      type_node = context.current_node->as<Type>();
    } else {
      if (PlaceholderType* placeholder = context.current_node->find_child<PlaceholderType>()) {
        type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
        context.current_node->replace_child(placeholder, type_node);
      } else {
        type_node = context.current_node->create_child<Type>();
      }
    }

    if (last_is_qualifier) {
      last->reparent(type_node);
    }

    Identifier* type_name = type_node->create_child<Identifier>();
    type_name->name = context.copy_node_content(context.node);

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
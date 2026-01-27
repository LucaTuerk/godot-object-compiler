//
// Created by luca on 18.01.26.
//

#include "list_handler.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  bool ListHandler::handles_node(TSNode& node, const String& type) { return string_contains(type, "list"); }

  NextStep ListHandler::handle(ParserContext& context) {
    String type = ts_node_type(context.node);
    if (type == "field_declaration_list") {
      context.current_node = context.current_node->create_child<Body>();
    } else if (type == "declaration_list") {
      context.current_node = context.current_node->create_child<Body>();
    } else if (type == "parameter_list") {
      if (context.current_node->is<Attribute>()) {
        // Parameters are handled by attributes parser.
        return STEP_OUT;
      }
      context.current_node = context.current_node->create_child<Parameters>();
    } else if (type == "template_argument_list") {
      context.current_node = context.current_node->create_child<TemplateParameters>();
    } else {
      context.current_node = context.current_node->create_child<Context>();
    }

    return STEP_INTO;
  }

}  // namespace GodotObjectCompiler
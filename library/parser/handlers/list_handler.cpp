//
// Created by luca on 18.01.26.
//

#include "list_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool ListHandler::handles_node(TSNode& node, const String& type) { return string_contains(type, "list"); }
  //
  // NextStep ListHandler::handle(ParserContext& context) {
  //   String type = ts_node_type(context.node);
  //   if (type == "field_declaration_list") {
  //     context.current_node = context.current_node->create_child<Body>();
  //   } else if (type == "declaration_list") {
  //     context.current_node = context.current_node->create_child<Body>();
  //   } else if (type == "parameter_list") {
  //     if (context.current_node->is<Attribute>()) {
  //       // Parameters are handled by attributes parser.
  //       return STEP_OUT;
  //     }
  //     context.current_node = context.current_node->create_child<Parameters>();
  //   } else if (type == "template_argument_list") {
  //     context.current_node = context.current_node->create_child<TemplateArguments>();
  //   } else {
  //     context.current_node = context.current_node->create_child<Context>();
  //   }
  //
  //   return STEP_INTO;
  // }

  bool ListHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    // TODO: Should the be string_suffix?
    return string_contains(current_src->type, "list");
  }

  ParserStep ListHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_target->is<Attribute>()) {
      // Parameters are handled by attributes parser.
      return ParserStep::StepOver();
    }

    if (current_src->type_in({"declaration_list", "field_declaration_list"})) {
      current_target = current_target->create_child<Body>();
    } else if (current_src->type == "parameter_list") {
      current_target = current_target->create_child<Parameters>();
    } else if (current_src->type == "template_argument_list") {
      current_target = current_target->create_child<TemplateArguments>();
    } else {
      current_target = current_target->create_child<Context>();
    }

    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
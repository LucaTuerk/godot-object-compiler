//
// Created by luca on 20.01.26.
//

#include "enum_handler.h"

#include "library/parser/tree_sitter_node.h"

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

  bool EnumHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"enum_specifier", "enumerator_list", "enumerator"});
  }

  ParserStep EnumHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->type == "enum_specifier") {
      current_target = current_target->create_child<Enum>();
    }
    if (current_src->type == "enumerator_list") {
      current_target = current_target->create_child<EnumValues>();
    }
    if (current_src->type == "enumerator") {
      current_target = current_target->create_child<EnumValue>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
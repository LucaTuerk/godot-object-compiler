
#include "sized_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  //
  // bool SizedSpecifierHandler::handles_node(TSNode& node, const String& type) {
  //   return type == "sized_type_specifier" || type == "long" || type == "short" || type == "unsigned" ||
  //          type == "signed";
  // }
  //
  // NextStep SizedSpecifierHandler::handle(ParserContext& context) {
  //   String type = ts_node_type(context.node);
  //
  //   if (type == "unsigned") {
  //     context.current_node->create_child<Unsigned>();
  //   } else if (type == "signed") {
  //     context.current_node->create_child<Signed>();
  //   } else if (type == "long") {
  //     context.current_node->create_child<Long>();
  //   } else if (type == "short") {
  //     context.current_node->create_child<Short>();
  //   } else if (type == "sized_type_specifier") {
  //     context.current_node = context.current_node->create_child<Type>();
  //     return STEP_INTO;
  //   }
  //
  //   return STEP_OVER;
  // }

  bool SizedSpecifierHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"sized_type_specifier", "long", "short", "unsigned", "signed"});
  }

  ParserStep SizedSpecifierHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->type == "unsigned") {
      current_target->create_child<Unsigned>();
    } else if (current_src->type == "signed") {
      current_target->create_child<Signed>();
    } else if (current_src->type == "long") {
      current_target->create_child<Long>();
    } else if (current_src->type == "short") {
      current_target->create_child<Short>();
    } else if (current_src->type == "sized_type_specifier") {
      current_target = current_target->create_child<Type>();
      return ParserStep::StepInto();
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
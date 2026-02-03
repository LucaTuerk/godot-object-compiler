#include "function_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool FieldHandler::handles_node(TSNode& node, const String& type) {
  //   return type == "field_declaration" || type == "declaration" || type == "function_definition" ||
  //          type == "expression_statement";
  // }
  //
  // bool has_decendant_of_type(TSNode node, const String& type) {
  //   if (ts_node_is_null(node)) {
  //     return false;
  //   }
  //
  //   for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
  //     TSNode child = ts_node_child(node, i);
  //     if (ts_node_type(child) == type || has_decendant_of_type(child, type)) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }
  //
  // NextStep FieldHandler::handle(ParserContext& context) {
  //   TSTreeCursor cursor = ts_tree_cursor_new(context.node);
  //
  //   if (has_decendant_of_type(context.node, "function_declarator")) {
  //     context.current_node = context.current_node->build_child<Function>();
  //   } else if (has_decendant_of_type(context.node, "call_expression")) {
  //     context.current_node = context.current_node->build_child<Function>();
  //   } else if (has_decendant_of_type(context.node, "class_specifier")) {
  //   } else if (has_decendant_of_type(context.node, "struct_specifier")) {
  //   } else if (has_decendant_of_type(context.node, "enum_specifier")) {
  //   } else {
  //     context.current_node = context.current_node->build_child<Field>();
  //   }
  //
  //   ts_tree_cursor_delete(&cursor);
  //   return STEP_INTO;
  // }

  bool FieldHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"field_declaration", "declaration", "function_definition", "expression_statement"});
  }

  ParserStep FieldHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->find_descendant(BFS, type_is("function_declarator")) != nullptr) {
      current_target = current_target->build_child<Function>();
    } else if (current_src->find_descendant(BFS, type_is("call_expression")) != nullptr) {
      current_target = current_target->build_child<Function>();
    } else if (current_src->find_descendant(BFS, type_in({"class_specifier", "struct_specifier", "enum_specifier"})) !=
               nullptr) {
    } else {
      current_target = current_target->build_child<Field>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
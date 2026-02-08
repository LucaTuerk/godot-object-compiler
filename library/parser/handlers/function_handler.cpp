#include "function_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool FieldHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type_in({"field_declaration", "declaration", "function_definition", "expression_statement"});
  }

  ParserStep FieldHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->find_descendant(BFS, type_is("function_declarator")) != nullptr) {
      r_current_target = r_current_target->build_child<Function>();
    } else if (p_current_src->find_descendant(BFS, type_is("call_expression")) != nullptr) {
      r_current_target = r_current_target->build_child<Function>();
    } else if (p_current_src->find_descendant(
                   BFS, type_in({"class_specifier", "struct_specifier", "enum_specifier"})) != nullptr) {
    } else {
      r_current_target = r_current_target->build_child<Field>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
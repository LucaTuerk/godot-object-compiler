#include "struct_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool StructHandler::handles_node(TSNode& node, const String& type) { return type == "struct_specifier"; }
  //
  // NextStep StructHandler::handle(ParserContext& context) {
  //   context.current_node = context.current_node->build_child<Struct>();
  //   return STEP_INTO;
  // }

  bool StructHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "struct_specifier";
  }

  ParserStep StructHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target = current_target->build_child<Struct>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
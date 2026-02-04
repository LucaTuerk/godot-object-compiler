#include "struct_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/struct.h"

namespace GodotObjectCompiler {

  bool StructHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "struct_specifier";
  }

  ParserStep StructHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target = current_target->build_child<Struct>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
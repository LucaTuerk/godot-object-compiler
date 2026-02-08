#include "struct_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/struct.h"

namespace GodotObjectCompiler {

  bool StructHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "struct_specifier";
  }

  ParserStep StructHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    r_current_target = r_current_target->build_child<Struct>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
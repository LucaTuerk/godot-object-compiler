#include "class_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"

namespace GodotObjectCompiler {

  bool ClassHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "class_specifier";
  }

  ParserStep ClassHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> found = current_src->find_descendant<TreeSitterNode>(BFS, type_is("field_declaration_list"));
    if (!found) {
      return ParserStep::StepOver();
    }

    current_target = current_target->build_child<Class>();
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
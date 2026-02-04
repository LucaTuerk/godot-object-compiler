#include "type_qualifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  ParserStep TypeQualifierHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->content() == "const") {
      current_target->create_child<Const>();
    } else if (current_src->content() == "mutable") {
      current_target->create_child<Mutable>();
    } else if (current_src->content() == "volatile") {
      current_target->create_child<Volatile>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
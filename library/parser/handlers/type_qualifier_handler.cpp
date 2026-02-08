#include "type_qualifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"

namespace GodotObjectCompiler {

  ParserStep TypeQualifierHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->content() == "const") {
      r_current_target->create_child<Const>();
    } else if (p_current_src->content() == "mutable") {
      r_current_target->create_child<Mutable>();
    } else if (p_current_src->content() == "volatile") {
      r_current_target->create_child<Volatile>();
    }
    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
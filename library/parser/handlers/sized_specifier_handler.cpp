
#include "sized_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  bool SizedSpecifierHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type_in({"sized_type_specifier", "long", "short", "unsigned", "signed"});
  }

  ParserStep SizedSpecifierHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (p_current_src->type == "unsigned") {
      r_current_target->create_child<Unsigned>();
    } else if (p_current_src->type == "signed") {
      r_current_target->create_child<Signed>();
    } else if (p_current_src->type == "long") {
      r_current_target->create_child<Long>();
    } else if (p_current_src->type == "short") {
      r_current_target->create_child<Short>();
    } else if (p_current_src->type == "sized_type_specifier") {
      r_current_target = r_current_target->create_child<Type>();
      return ParserStep::StepInto();
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
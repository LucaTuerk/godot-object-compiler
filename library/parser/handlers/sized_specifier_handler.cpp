
#include "sized_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  bool SizedSpecifierHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"sized_type_specifier", "long", "short", "unsigned", "signed"});
  }

  ParserStep SizedSpecifierHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
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
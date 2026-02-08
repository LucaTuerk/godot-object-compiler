
#include "type_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/struct.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  auto TypeHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) -> bool {
    return p_current_src->type_in(
        {"primitive_type", "qualified_identifier", "type_identifier", "trailing_return_type", "template_type"});
    // return string_suffix(current_src->type, "type") ||
    //       ;
  }

  ParserStep TypeHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if ((r_current_target->is<Class>() || r_current_target->is<Struct>() || r_current_target->is<Enum>()) &&
        (r_current_target->find_child<Identifier>() == nullptr)) {
      r_current_target->create_child<Identifier>(p_current_src->content());
      return ParserStep::StepOver();
    }

    if (p_current_src->type == "template_type") {
      // current_target = current_target->create_child<Type>();
      r_current_target = r_current_target->create_child<Type>();
      return ParserStep::Undecided();
    }

    Ref<Node> last = r_current_target->get_child(-1);
    bool last_is_qualifier = last && last->is<TypeQualifier>();

    Ref<Type> type_node;
    if (r_current_target->is<Type>()) {
      type_node = r_current_target->as<Type>();
    } else {
      if (Ref<PlaceholderType> placeholder = r_current_target->find_child<PlaceholderType>()) {
        type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
        r_current_target->replace_child(placeholder, type_node);
      } else {
        type_node = r_current_target->create_child<Type>();
      }
    }

    if (last_is_qualifier) {
      last->reparent(type_node);
    }

    // current_target = type_node;
    type_node->create_child<Identifier>(p_current_src->content());

    if (p_current_src->type == "qualified_identifier") {
      return ParserStep::StepOver();
    } else {
      return ParserStep::Undecided();
    }
  }

}  // namespace GodotObjectCompiler
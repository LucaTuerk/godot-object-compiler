
#include "type_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/struct.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  auto TypeHandler::handles_node(const Ref<TreeSitterNode>& current_src) -> bool {
    return current_src->type_in(
        {"primitive_type", "qualified_identifier", "type_identifier", "trailing_return_type", "template_type"});
    // return string_suffix(current_src->type, "type") ||
    //       ;
  }

  ParserStep TypeHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if ((current_target->is<Class>() || current_target->is<Struct>() || current_target->is<Enum>()) &&
        (current_target->find_child<Identifier>() == nullptr)) {
      current_target->create_child<Identifier>(current_src->content());
      return ParserStep::StepOver();
    }

    if (current_src->type == "template_type") {
      // current_target = current_target->create_child<Type>();
      current_target = current_target->create_child<Type>();
      return ParserStep::Undecided();
    }

    Ref<Node> last = current_target->get_child(-1);
    bool last_is_qualifier = last && last->is<TypeQualifier>();

    Ref<Type> type_node;
    if (current_target->is<Type>()) {
      type_node = current_target->as<Type>();
    } else {
      if (Ref<PlaceholderType> placeholder = current_target->find_child<PlaceholderType>()) {
        type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
        current_target->replace_child(placeholder, type_node);
      } else {
        type_node = current_target->create_child<Type>();
      }
    }

    if (last_is_qualifier) {
      last->reparent(type_node);
    }

    // current_target = type_node;
    type_node->create_child<Identifier>(current_src->content());

    if (current_src->type == "qualified_identifier") {
      return ParserStep::StepOver();
    } else {
      return ParserStep::Undecided();
    }
  }

}  // namespace GodotObjectCompiler
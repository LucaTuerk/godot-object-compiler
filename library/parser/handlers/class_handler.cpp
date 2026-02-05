#include "class_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool ClassHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"class_specifier", "base_class_clause"});
  }

  ParserStep ClassHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_src->type == "class_specifier") {
      Ref<TreeSitterNode> found = current_src->find_descendant<TreeSitterNode>(BFS, type_is("field_declaration_list"));
      if (!found) {
        return ParserStep::StepOver();
      }

      Ref<TreeSitterNode> identifier = current_src->find_child<TreeSitterNode>(BFS, type_is("type_identifier"));
      if (!identifier) {
        print_err("A class with no identifier.");
        print_ln(current_src->pretty_print());
        return ParserStep::StepOver();
      }

      Ref<Node> previous_node = current_target->get_child(-1);
      current_target = current_target->build_child<Class>();
      current_target->create_child<Identifier>(identifier->content());
      identifier->set_handled();
      if (previous_node && previous_node->is<TemplateParameters>()) {
        current_target->add_child(previous_node);
      }
    }

    if (current_src->type == "base_class_clause") {
      current_target = current_target->build_child<BaseClasses>();
    }

    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
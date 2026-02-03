
#include "define_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  bool DefineHandler::handles_node(TSNode& node, const String& type) {
    return type == "preproc_def" || type == "preproc_function_def";
  }

  NextStep DefineHandler::handle(ParserContext& context) {
    TSNode identifier = find_child_of_type(context.node, "identifier");
    if (!ts_node_is_null(identifier)) {
      const Ref<Define> define = node_new<Define>();
      define->build_child<Identifier>(context.copy_node_content(identifier));
      if (String(ts_node_type(context.node)) == "preproc_function_def") {
        define->build_child<Arguments>();
      }
      context.current_node->add_child(define);
    }

    return STEP_OVER;
  }

  bool DefineHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"preproc_def", "preproc_function_def"});
  }

  ParserStep DefineHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> identifier = current_src->find_child<TreeSitterNode>(0, type_is("identifier"));
    if (identifier) {
      current_target = build<Define>().with_child<Identifier>(identifier->content());
      if (current_src->type == "preproc_function_def") {
        current_target->build_child<Arguments>();
      }
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
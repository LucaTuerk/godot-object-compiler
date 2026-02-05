
#include "define_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool DefineHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"preproc_def", "preproc_function_def"});
  }

  ParserStep DefineHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<TreeSitterNode> identifier = current_src->find_child<TreeSitterNode>(0, type_is("identifier"));
    if (identifier) {
      Ref<Define> define = current_target->build_child<Define>().with_child<Identifier>(identifier->content());
      if (current_src->type == "preproc_function_def") {
        define->build_child<Arguments>();
      }
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
#include "preprocessor_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/preprocessor_define.h"

namespace GodotObjectCompiler {

  bool PreprocessorHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type_in({"preproc_def", "preproc_function_def"});
  }

  ParserStep PreprocessorHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (Ref<TreeSitterNode> identifier = p_current_src->find_child(0, type_is("identifier"))) {
      r_current_target->create_child<PreprocessorDefine>(identifier->content());
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
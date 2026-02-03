//
// Created by luca on 20.01.26.
//

#include "preprocessor_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/preprocessor_define.h"

namespace GodotObjectCompiler {

  // bool PreprocessorHandler::handles_node(TSNode& node, const String& type) {
  //   return type == "preproc_def" || type == "preproc_function_def";
  // }
  //
  // NextStep PreprocessorHandler::handle(ParserContext& context) {
  //   bool success;
  //   TSNode identifier = context.get_child_node_by_type("identifier", success);
  //   if (success) {
  //     context.current_node->create_child<PreprocessorDefine>(context.copy_node_content(identifier));
  //   }
  //   return STEP_OVER;
  // }

  bool PreprocessorHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type_in({"preproc_def", "preproc_function_def"});
  }

  ParserStep PreprocessorHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (Ref<TreeSitterNode> identifier = current_src->find_child(0, type_is("identifier"))) {
      current_target->create_child<PreprocessorDefine>(identifier->content());
    }
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
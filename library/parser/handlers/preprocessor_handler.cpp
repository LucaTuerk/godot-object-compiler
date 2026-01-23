//
// Created by luca on 20.01.26.
//

#include "preprocessor_handler.h"

#include "library/tree/syntax/preprocessor_define.h"

namespace GodotObjectCompiler {

  bool PreprocessorHandler::handles_node(TSNode& node, const String& type) {
    return type == "preproc_def" || type == "preproc_function_def";
  }

  NextStep PreprocessorHandler::handle(ParserContext& context) {
    bool success;
    TSNode identifier = context.get_child_node_by_type("identifier", success);
    if (success) {
      context.current_node->create_child<PreprocessorDefine>(context.copy_node_content(identifier));
    }
    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
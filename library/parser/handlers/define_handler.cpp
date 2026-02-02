
#include "define_handler.h"

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

}  // namespace GodotObjectCompiler
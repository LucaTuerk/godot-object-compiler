#include "include_handler.h"

namespace GodotObjectCompiler {

  bool IncludeHandler::handles_node(TSNode& node, const String& type) { return type == "preproc_include"; }

  NextStep IncludeHandler::handle(ParserContext& context) {
    bool success = false, system_success = false;
    TSNode content = context.get_descendant_by_type("string_content", success);
    if (!success) {
      content = context.get_descendant_by_type("system_lib_string", system_success);
    }

    if (success) {
      Include* include = context.current_node->create_child<Include>(context.copy_node_content(content), false);
    } else if (system_success) {
      Include* include = context.current_node->create_child<Include>(context.copy_node_content(content), true);
    } else {
      print_err("Could not find string_literal node");
    }

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
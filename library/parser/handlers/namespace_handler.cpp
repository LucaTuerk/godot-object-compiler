
#include "library/parser/handlers/namespace_handler.h"

#include "library/core/core.h"
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  bool NamespaceHandler::handles_node(TSNode& node, const String& type) {
    return String(ts_node_type(node)) == "namespace_definition";
  }

  NextStep NamespaceHandler::handle(ParserContext& context) {
    context.current_node = context.create_namespace();
    return STEP_INTO;
  }

}  // namespace GodotObjectCompiler
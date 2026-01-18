#include "library/parser/tree_sitter_parser/handlers/namespace_handler.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

	bool NamespaceHandler::handles_node(TSNode &node, const String& type) {
		return String(ts_node_type(node)) == "namespace_definition";
	}

	NextStep NamespaceHandler::handle(ParserContext& context) {
		context.create_namespace();
		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
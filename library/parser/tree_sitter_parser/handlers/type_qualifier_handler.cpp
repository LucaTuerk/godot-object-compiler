//
// Created by luca on 18.01.26.
//

#include "type_qualifier_handler.h"

namespace GodotObjectCompiler {

	bool TypeQualifierHandler::handles_node(TSNode &node, const String &type) {
		return type == "type_qualifier";
	}

	NextStep TypeQualifierHandler::handle(ParserContext &context) {
		String content = context.copy_node_content(context.node);

		if (content == "const") {
			context.current_node->create_child<Const>();
		} else if (content == "mutable") {
			context.current_node->create_child<Mutable>();
		} else if (content == "volatile") {
			context.current_node->create_child<Volatile>();
		}

		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
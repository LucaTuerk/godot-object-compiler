//
// Created by luca on 17.01.26.
//

#include "class_handler.h"

namespace GodotObjectCompiler {

	bool ClassHandler::handles_node(TSNode &node, const String& type) {
		return String(ts_node_type(node)) == "class_specifier";
	}

	NextStep ClassHandler::handle(ParserContext &context) {
		context.current_node = context.create_class();
		AccessSpecifier* as = context.current_node->create_child<AccessSpecifier>();
		as->type = AccessSpecifier::PRIVATE;
		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
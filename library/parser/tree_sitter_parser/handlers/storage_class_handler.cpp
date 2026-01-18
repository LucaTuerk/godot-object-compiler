//
// Created by luca on 17.01.26.
//

#include "storage_class_handler.h"

namespace GodotObjectCompiler {

	bool StorageClassHandler::handles_node(TSNode &node, const String &type) {
		return string_contains(type, "storage_class_specifier");
	}

	NextStep StorageClassHandler::handle(ParserContext &context) {
		if (context.copy_node_content(context.node) == "static") {
			context.current_node->create_child<Static>();
		}
		return STEP_OVER;
	}

} //namespace GodotObjectCompiler
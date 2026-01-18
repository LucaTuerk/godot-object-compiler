//
// Created by luca on 17.01.26.
//

#include "virtual_handler.h"

#include "library/tree/modifiers.h"

namespace GodotObjectCompiler {

	bool VirtualHandler::handles_node(TSNode &node, const String &type) {
		return string_contains(type, "virtual") || string_contains(type, "virtual_specifier");
	}

	NextStep VirtualHandler::handle(ParserContext &context) {
		String type = ts_node_type(context.node);

		if (string_contains(type, "virtual")) {
			context.current_node->create_child<Virtual>();
		} else if (string_contains(type, "virtual_specifier")) {
			if (context.copy_node_content(context.node) == "override") {
				context.current_node->create_child<Override>();
			}
		}

		return STEP_OVER;
	}

} //namespace GodotObjectCompiler
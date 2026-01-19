#include "function_handler.h"

namespace GodotObjectCompiler {

	bool FieldHandler::handles_node(TSNode &node, const String &type) {
		return type == "field_declaration" || type == "declaration";
	}

	bool has_decendant_of_type(TSNode node, const String &type) {
		if (ts_node_is_null(node)) {
			return false;
		}

		for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
			TSNode child = ts_node_child(node, i);
			if (ts_node_type(child) == type || has_decendant_of_type(child, type)) {
				return true;
			}
		}
		return false;
	}

	NextStep FieldHandler::handle(ParserContext &context) {
		TSTreeCursor cursor = ts_tree_cursor_new(context.node);

		if (has_decendant_of_type(context.node,"function_declarator")) {
			context.current_node = context.create_function();
		} else if (has_decendant_of_type(context.node, "class_specifier")) {
		} else if (has_decendant_of_type(context.node, "struct_specifier")) {
		} else {
			context.current_node = context.create_field();
		}

		ts_tree_cursor_delete(&cursor);
		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
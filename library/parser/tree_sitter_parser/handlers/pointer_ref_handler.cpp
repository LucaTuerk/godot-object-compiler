#include "pointer_ref_handler.h"
#include "library/parser/tree_sitter_parser/node_handler.h"
#include "library/tree/type.h"

namespace GodotObjectCompiler {

	bool PointerRefHandler::handles_node(TSNode &node, const String &type) {
		_current = NONE;
		if (string_only_contains(type, '&')) {
			_current = REFERENCE;
		} else if (string_only_contains(type, '*')) {
			_current = POINTER;
		}

		return _current != NONE;
	}

	NextStep PointerRefHandler::handle(ParserContext &context) {
		String type = ts_node_type(context.node);

		Context *type_node = context.current_node->find_child<Type>();
		if (!type_node) {
			type_node = context.current_node;
		}

		for (Size i = 0; i < type.length(); ++i) {
			switch (_current) {
				case POINTER:
					type_node->create_child<Pointer>();
					break;
				case REFERENCE:
					type_node->create_child<Reference>();
					break;
				default: {};
			}
		}

		return UNDECIDED;
	}

} //namespace GodotObjectCompiler
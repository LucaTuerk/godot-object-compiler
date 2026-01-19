#include "struct_handler.h"

namespace GodotObjectCompiler {

	bool StructHandler::handles_node(TSNode &node, const String &type) {
		return type == "struct_specifier";
	}

	NextStep StructHandler::handle(ParserContext &context) {
		context.current_node = context.create_struct();
		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
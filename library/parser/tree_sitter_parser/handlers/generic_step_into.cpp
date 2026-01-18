//
// Created by luca on 17.01.26.
//

#include "generic_step_into.h"

namespace GodotObjectCompiler {

	bool GenericStepInto::handles_node(TSNode &node, const String &type) {
		return string_contains(type, "_list");
	}

	NextStep GenericStepInto::handle(ParserContext &context) {
		return STEP_INTO;
	}

} //namespace GodotObjectCompiler
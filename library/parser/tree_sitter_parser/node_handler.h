#pragma once

#include "library/core/core.h"
#include "library/tree/all.h"
#include "parser_context.h"

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

namespace GodotObjectCompiler {
	enum NextStep {
		UNDECIDED,
		STEP_INTO,
		STEP_OVER,
		STEP_OUT
	};

    class INodeHandler {
        public:
			virtual ~INodeHandler() = default;

			virtual bool handles_node(TSNode &node, const String &type) = 0;
            virtual NextStep handle(ParserContext& context) = 0;
    };
}

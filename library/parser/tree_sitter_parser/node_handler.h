#pragma once

#include "library/core/core.h"
#include "library/tree/all.h"
#include "parser_context.h"
#include "library/parser/tree_sitter_parser.h"

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

namespace GodotObjectCompiler {
	enum NextStep {
		UNDECIDED,
		STEP_INTO,
		STEP_OVER,
		STEP_OUT
	};

#define NODE_HANDLER(name) \
	static inline bool __registered___ = TreeSitterParser::register_handler<name>();

    class INodeHandler {
        public:
			virtual ~INodeHandler() = default;

			virtual bool handles_node(TSNode &node, const String &type) = 0;
            virtual NextStep handle(ParserContext& context) = 0;
    };
}

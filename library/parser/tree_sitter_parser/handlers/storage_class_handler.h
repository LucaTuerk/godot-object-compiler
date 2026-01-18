#pragma once
#include "library/parser/tree_sitter_parser/node_handler.h"

namespace GodotObjectCompiler {

	class StorageClassHandler : public INodeHandler {
		public:
			~StorageClassHandler() override = default;
			bool handles_node(TSNode &node, const String &type) override;
			NextStep handle(ParserContext &context) override;
};

} //namespace GodotObjectCompiler

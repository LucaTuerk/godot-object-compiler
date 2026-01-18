#pragma once

#include "../node_handler.h"

namespace GodotObjectCompiler {

	class GenericStepInto : public INodeHandler {
		NODE_HANDLER(GenericStepInto)
public:
	~GenericStepInto() override = default;
	bool handles_node(TSNode &node, const String &type) override;
	NextStep handle(ParserContext &context) override;
};

} //namespace GodotObjectCompiler

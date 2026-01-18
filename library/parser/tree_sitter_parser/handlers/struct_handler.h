#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

class StructHandler : public INodeHandler {
public:
	~StructHandler() override = default;
	bool handles_node(TSNode &node, const String &type) override;
	NextStep handle(ParserContext &context) override;
};

} //namespace GodotObjectCompiler

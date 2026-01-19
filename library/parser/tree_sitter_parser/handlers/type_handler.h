#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

INTO(trailing_return_type);
INTO(type_descriptor);

SIMPLE_GENERATE(placeholder_type_specifier, PlaceholderType, STEP_OVER);

class TypeHandler : public INodeHandler {
	NODE_HANDLER(TypeHandler)

public:
	~TypeHandler() override = default;
	bool handles_node(TSNode &node, const String &type) override;
	NextStep handle(ParserContext &context) override;
};

} //namespace GodotObjectCompiler

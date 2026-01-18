//
// Created by luca on 17.01.26.
//

#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

class ClassHandler : public INodeHandler {
public:
	~ClassHandler() override = default;
	bool handles_node(TSNode &node, const String& type) override;
	NextStep handle(ParserContext &context) override;
};

} //namespace GodotObjectCompiler


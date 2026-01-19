#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {
  class NamespaceHandler : public INodeHandler {
    NODE_HANDLER(NamespaceHandler);

   public:
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };
}  // namespace GodotObjectCompiler

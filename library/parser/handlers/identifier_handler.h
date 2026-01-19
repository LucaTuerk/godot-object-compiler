#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class IdentifierHandler : public INodeHandler {
    NODE_HANDLER(IdentifierHandler)

   public:
    ~IdentifierHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

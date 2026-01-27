#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class IdentifierHandler : public INodeHandler {
    NODE_HANDLER(IdentifierHandler)

   public:

    IdentifierHandler();
    ~IdentifierHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;

    NextStep handle(ParserContext& context) override;
    NextStep handle_known_attribute(ParserContext& context, const String& macro);
  };

}  // namespace GodotObjectCompiler

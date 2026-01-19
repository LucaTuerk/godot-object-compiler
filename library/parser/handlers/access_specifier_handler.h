#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class AccessSpecifierHandler : public INodeHandler {
    NODE_HANDLER(AccessSpecifierHandler)

   public:
    ~AccessSpecifierHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

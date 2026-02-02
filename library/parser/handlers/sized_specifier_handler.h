
#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

  class SizedSpecifierHandler : public INodeHandler {
    NODE_HANDLER(SizedSpecifierHandler);

   public:

    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

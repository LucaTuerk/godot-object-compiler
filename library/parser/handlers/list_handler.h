#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class ListHandler : public INodeHandler {
    NODE_HANDLER(ListHandler)
   public:

    ~ListHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

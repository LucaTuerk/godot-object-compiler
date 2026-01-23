#pragma once
#include "library/core/core.h"
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class ErrorHandler : public INodeHandler {
    NODE_HANDLER(ErrorHandler)
   public:
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

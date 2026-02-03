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

  class ErrorHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(ErrorHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

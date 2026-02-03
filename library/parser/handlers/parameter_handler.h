#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class ParameterHandler : public INodeHandler {  //"parameter_declaration"
    NODE_HANDLER(ParameterHandler)

   public:

    ~ParameterHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

  class ParameterHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(ParameterHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

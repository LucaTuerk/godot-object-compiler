
#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

  //
  // class SizedSpecifierHandler : public INodeHandler {
  //   NODE_HANDLER(SizedSpecifierHandler);
  //
  //  public:
  //
  //   bool handles_node(TSNode& node, const String& type) override;
  //   NextStep handle(ParserContext& context) override;
  // };

  class SizedSpecifierHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(SizedSpecifierHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

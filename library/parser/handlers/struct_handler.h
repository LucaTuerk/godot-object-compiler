#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  // class StructHandler : public INodeHandler {
  //   NODE_HANDLER(StructHandler)
  //
  //  public:
  //
  //   ~StructHandler() override = default;
  //   bool handles_node(TSNode& node, const String& type) override;
  //   NextStep handle(ParserContext& context) override;
  // };

  class StructHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(StructHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

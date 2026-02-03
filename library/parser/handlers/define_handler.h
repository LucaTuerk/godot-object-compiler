
#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  // class DefineHandler : public INodeHandler {
  //   NODE_HANDLER(DefineHandler)
  //
  //  public:
  //
  //   bool handles_node(TSNode& node, const String& type) override;
  //   NextStep handle(ParserContext& context) override;
  // };

  class DefineHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(DefineHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

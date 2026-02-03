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

  class IdentifierHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(IdentifierHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
    ParserStep handle_known_attribute(
        const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target, const String& macro);
  };

}  // namespace GodotObjectCompiler

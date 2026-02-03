#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class TypeQualifierHandler : public INodeHandler {
    NODE_HANDLER(TypeQualifierHandler)

   public:

    ~TypeQualifierHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

  class TypeQualifierHandlerV2 : public INodeHandlerV2 {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

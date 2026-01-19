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

}  // namespace GodotObjectCompiler

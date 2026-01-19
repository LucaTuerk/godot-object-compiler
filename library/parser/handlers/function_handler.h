#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class FieldHandler : public INodeHandler {
    NODE_HANDLER(FieldHandler)

   public:
    ~FieldHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

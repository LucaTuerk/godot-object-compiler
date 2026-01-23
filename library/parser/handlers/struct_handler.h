#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class StructHandler : public INodeHandler {
    NODE_HANDLER(StructHandler)

   public:

    ~StructHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

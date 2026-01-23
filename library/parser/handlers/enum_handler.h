#pragma once

#include "../node_handler.h"

namespace GodotObjectCompiler {

  class EnumHandler : public INodeHandler {
    NODE_HANDLER(EnumHandler)  // enum_specifier

   public:
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

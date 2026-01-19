#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class StorageClassHandler : public INodeHandler {
    NODE_HANDLER(StorageClassHandler)

   public:
    ~StorageClassHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

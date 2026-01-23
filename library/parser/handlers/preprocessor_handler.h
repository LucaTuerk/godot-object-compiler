#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

  class PreprocessorHandler : public INodeHandler {
    NODE_HANDLER(PreprocessorHandler);

   public:

    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class ParameterHandler : public INodeHandler {  //"parameter_declaration"
    NODE_HANDLER(ParameterHandler)

   public:

    ~ParameterHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

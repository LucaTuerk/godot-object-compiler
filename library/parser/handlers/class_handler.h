#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class ClassHandler : public INodeHandler {
    NODE_HANDLER(ClassHandler)

   public:

    ~ClassHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

}  // namespace GodotObjectCompiler

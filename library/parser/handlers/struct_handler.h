#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class StructHandler : public INodeHandler {
    NODE_HANDLER(StructHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler


#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class SizedSpecifierHandler : public INodeHandler {
    NODE_HANDLER(SizedSpecifierHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) override;
  };

}  // namespace GodotObjectCompiler

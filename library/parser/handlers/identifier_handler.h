#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class IdentifierHandler : public INodeHandler {
    NODE_HANDLER(IdentifierHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
    ParserStep handle_known_attribute(
        const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target, const String& macro);
  };

}  // namespace GodotObjectCompiler

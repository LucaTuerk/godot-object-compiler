
#pragma once
#include "../node_handler.h"

namespace GodotObjectCompiler {

  class IfDefHandler : INodeHandler {
   public:

    void set_step(ParserStep step);
    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;

   private:

    ParserStep step = ParserStep::StepOver();
  };

}  // namespace GodotObjectCompiler


#include "ifdef_handler.h"

#include <utility>

namespace GodotObjectCompiler {

  void IfDefHandler::set_step(ParserStep step) { this->step = std::move(step); }

  bool IfDefHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "preproc_ifdef";
  }

  ParserStep IfDefHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) { return step; }

}  // namespace GodotObjectCompiler
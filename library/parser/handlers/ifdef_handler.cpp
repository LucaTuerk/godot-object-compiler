
#include "ifdef_handler.h"

#include <utility>

namespace GodotObjectCompiler {

  void IfDefHandler::set_step(ParserStep step) { this->step = std::move(step); }

  bool IfDefHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "preproc_ifdef";
  }

  ParserStep IfDefHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    return step;
  }

}  // namespace GodotObjectCompiler
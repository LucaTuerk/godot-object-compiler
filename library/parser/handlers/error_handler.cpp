#include "error_handler.h"

#include "library/core/string_writer.h"
#include "library/parser/parser_context.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/node.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  bool ErrorHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) { return p_current_src->type == "ERROR"; }

  ParserStep ErrorHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    r_current_target->create_child<ParserError>(ERROR, "TreeSitterParser", "", p_current_src->context->file_path,
        p_current_src->context->buffer, p_current_src->start_point.row + 1, p_current_src->start_point.column + 1);
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
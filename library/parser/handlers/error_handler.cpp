#include "error_handler.h"

#include "library/core/string_writer.h"
#include "library/parser/parser_context.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/node.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  bool ErrorHandler::handles_node(const Ref<TreeSitterNode>& current_src) { return current_src->type == "ERROR"; }

  ParserStep ErrorHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target->create_child<ParserError>(ERROR, "TreeSitterParser", "", current_src->context->file_path,
        current_src->context->buffer, current_src->start_point.row + 1, current_src->start_point.column + 1);
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
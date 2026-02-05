
#include "print_parsed.h"

#include "library/core/helpers.h"
#include "library/parser/parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> PrintParsed::run(ApplicationContext& context) {
    if (context.program_arguments.size() != 1) {
      return node_new<ProgramError>(
          ERROR, format("Invalid argument count for program %s. Expected 1 path argument.", get_type_static().c_str()));
    }

    auto path = path_absolute(context.program_arguments[0]);

    if (!file_exists(path)) {
      return node_new<ProgramError>(
          ERROR, format("Invalid path argument for program %s. File does not exist.", get_type_static().c_str()));
    }

    ParserContext parser_context(read_file(path));
    if (!parser_context.is_valid()) {
      return node_new<ProgramError>(ERROR, format("Failed to parse file %s.", path.c_str()));
    }

    print_ln(parser_context.current_src->pretty_print());
    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
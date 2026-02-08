
#include "print_tranformed.h"

#include "library/core/helpers.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> PrintTransformed::run(ApplicationContext& p_context) {
    if (p_context.program_arguments.size() != 1) {
      return node_new<ProgramError>(
          ERROR, format("Invalid argument count for program %s. Expected 1 path argument.", get_type_static().c_str()));
    }

    auto path = path_absolute(p_context.program_arguments[0]);

    if (!file_exists(path)) {
      return node_new<ProgramError>(
          ERROR, format("Invalid path argument for program %s. File does not exist.", get_type_static().c_str()));
    }

    TreeSitterParser parser;
    Ref<Namespace> ns = node_new<Namespace>();
    if (parser.parse_file(path, ns) != ParserError::OK) {
      return node_new<ProgramError>(ERROR, format("Failed to parse file %s.", path.c_str()));
    }

    print_ln(ns->pretty_print());
    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
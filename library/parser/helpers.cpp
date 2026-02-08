#include "helpers.h"

#include "library/execution_context.h"

namespace GodotObjectCompiler {

  String Parser::Helpers::remove_macros(const String& p_input) {
    String output = p_input;

    for (const String& macro : ExecutionContext::instance()->get_remove_macros()) {
      output = string_replace(output, macro, "");
    }

    return output;
  }

}  // namespace GodotObjectCompiler
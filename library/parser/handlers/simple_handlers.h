#pragma once

#include "../node_handler.h"
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  // DO NOT IMPLEMENT
  SKIP(template_declaration);
  SKIP(conditional_expression);
  SKIP(compound_statement);

  // MAYBE IMPLEMENT
  SKIP(number_literal);

  // SKIP(call_expression);
  SKIP(preproc_call);
  // SKIP(preproc_def);
  SKIP(preproc_ifdef);
  // SKIP(preproc_function_def);

  SKIP(operator);

}  // namespace GodotObjectCompiler

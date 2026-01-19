#pragma once

#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  // DO NOT IMPLEMENT
  SKIP(template_declaration);

  // MAYBE IMPLEMENT
  SKIP(preproc_call);
  SKIP(preproc_def);
  SKIP(preproc_function_def);

  // TO BE IMPLEMENTED
  SKIP(enum_specifier);
  SKIP(preproc_include);

}  // namespace GodotObjectCompiler

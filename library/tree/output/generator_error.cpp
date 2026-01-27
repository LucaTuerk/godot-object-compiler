
#include "generator_error.h"

namespace GodotObjectCompiler {

  String GeneratorError::to_string() const { return "GeneratorError:\n\t" + generator_name + "\n\t\t" + error_message; }

  bool GeneratorError::copy_to(Ref<Node> other) const {
    COPY_GUARD(GeneratorError, Context)
    target->generator_name = generator_name;
    target->error_message = error_message;
    return true;
  }

}  // namespace GodotObjectCompiler
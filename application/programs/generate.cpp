
#include "generate.h"

#include "generate_bindings.h"
#include "generate_type_db.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> Generate::run(ApplicationContext& context) {
    GenerateTypeDB generate_type_db;
    Ref<ProgramError> generate_type_db_error = generate_type_db.run(context);
    if (generate_type_db_error != ProgramError::OK) {
      return generate_type_db_error;
    }

    GenerateBindings generate_bindings;
    Ref<ProgramError> generate_bindings_error = generate_bindings.run(context);
    if (generate_type_db_error != ProgramError::OK) {
      return generate_bindings_error;
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
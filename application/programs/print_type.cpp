
#include "print_type.h"

#include "library/type_db.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> PrintType::run(ApplicationContext& context) {
    if (context.program_arguments.empty()) {
      return make_ref<ProgramError>(
          ERROR, "No type name provided. Please specify one or more types by their fully qualified name.");
    }

    for (const String& name : context.program_arguments) {
      Ref<Node> type_data = TypeDB::instance()->get_type_data(name);
      if (!type_data) {
        return make_ref<ProgramError>(ERROR, "Unknown type name: " + name);
      }

      print_ln(type_data->pretty_print());
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
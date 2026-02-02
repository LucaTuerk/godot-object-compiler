
#include "program.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  bool Programs::register_program(const Ref<IProgram>& program) {
    if (_registered_programs.find(program->program_name()) != _registered_programs.end()) {
      return false;
    }

    ProgramPath path = string_split(program->program_name(), "/");
    _programs[path] = program;
    return true;
  }

  Ref<IProgram> Programs::find_program(const Vector<String>& application_arguments, Vector<String>& program_arguments) {
    Ref<IProgram> found_program;

    Size current_overlap = 0;
    for (const auto& [path, program] : _programs) {
      Size matching = overlap(path, application_arguments);
      if (matching == path.size() && matching > current_overlap) {
        found_program = program;
        program_arguments = {};

        for (Size i = matching; i < application_arguments.size(); i++) {
          program_arguments.push_back(application_arguments[i]);
        }
      }
    }

    return found_program;
  }

  const Dictionary<ProgramPath, Ref<IProgram>>& Programs::get_programs() { return _programs; }

  Size Programs::overlap(const ProgramPath& a, const ProgramPath& b) {
    Size overlap = 0;
    for (Size i = 0; i < a.size() && i < b.size(); i++) {
      if (a[i] != b[i]) {
        break;
      }
      overlap++;
    }
    return overlap;
  }

}  // namespace GodotObjectCompiler


#include "program.h"

namespace GodotObjectCompiler {

  bool Programs::register_program(const Ref<IProgram>& program) {
    _programs[program->program_name()] = program;
    return true;
  }

}  // namespace GodotObjectCompiler

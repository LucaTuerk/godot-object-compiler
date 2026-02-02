
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class GenerateAssumptions : public IProgram {
    PROGRAM(GenerateAssumptions, "generate/assumptions");

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

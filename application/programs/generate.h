
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class Generate : public IProgram {
    PROGRAM(Generate, "generate");

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

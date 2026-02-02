
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class Init : public IProgram {
    PROGRAM(Init, "init");

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler


#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class Init : public IProgram {
    PROGRAM(Init, "init");

   public:

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

}  // namespace GodotObjectCompiler

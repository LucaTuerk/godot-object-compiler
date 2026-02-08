#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class InitLocalResources : public IProgram {
    PROGRAM(InitLocalResources, "init/local_resources");

   public:

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

}  // namespace GodotObjectCompiler

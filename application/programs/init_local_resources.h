#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class InitLocalResources : public IProgram {
    PROGRAM(InitLocalResources, "init/local_resources");

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

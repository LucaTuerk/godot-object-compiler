
#pragma once
#include "library/core/core.h"
#include "program.h"

namespace GodotObjectCompiler {

  class ApplicationContext;

  class GenerateTypeDB : public IProgram {
    PROGRAM(GenerateTypeDB, "generate/type_db");

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

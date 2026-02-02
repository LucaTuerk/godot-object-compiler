
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class Clear : public IProgram {
    PROGRAM(Clear, "clear")

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

  class ClearGenerated : public IProgram {
    PROGRAM(ClearGenerated, "clear/generated")

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

  class ClearCache : public IProgram {
    PROGRAM(ClearCache, "clear/cache")

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

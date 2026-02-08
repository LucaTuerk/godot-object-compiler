
#pragma once
#include "library/core/core.h"
#include "program.h"

namespace GodotObjectCompiler {

  class PrintType : public IProgram {
    PROGRAM(PrintType, "print/type");

   public:

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

}  // namespace GodotObjectCompiler

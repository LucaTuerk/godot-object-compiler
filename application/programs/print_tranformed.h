
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class PrintTransformed : public IProgram {
    PROGRAM(PrintTransformed, "print/transformed")

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

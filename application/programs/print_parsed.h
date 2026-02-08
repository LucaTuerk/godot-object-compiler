
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class PrintParsed : public IProgram {
    PROGRAM(PrintParsed, "print/parsed")

   public:

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

}  // namespace GodotObjectCompiler

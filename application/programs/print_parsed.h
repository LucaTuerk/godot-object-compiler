
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class PrintParsed : public IProgram {
    PROGRAM(PrintParsed, "print/parsed")

   public:

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

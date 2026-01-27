
#pragma once
#include "library/core/core.h"
#include "program.h"

namespace GodotObjectCompiler {

  class ApplicationContext;

  class GenerateTypeDB : public IProgram {
    PROGRAM("generate_type_db");

   public:

    int run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

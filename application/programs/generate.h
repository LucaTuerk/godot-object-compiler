
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class Generate : public IProgram {
    PROGRAM("generate")

   public:

    String file_id(const String& file_name);
    String generated_macro_name(const String& file, Size line);

    int run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

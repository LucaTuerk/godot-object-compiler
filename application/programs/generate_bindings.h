
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class GenerateBindings : public IProgram {
    PROGRAM(GenerateBindings, "generate/bindings")

   public:

    String file_id(const String& file_name);
    String generated_macro_name(const String& file, Size line);

    Ref<ProgramError> run(ApplicationContext& context) override;
  };

}  // namespace GodotObjectCompiler

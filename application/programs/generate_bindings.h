
#pragma once
#include "program.h"

namespace GodotObjectCompiler {

  class GenerateBindings : public IProgram {
    PROGRAM(GenerateBindings, "generate/bindings")

   public:

    String file_id(const String& p_file_name);

    String generated_macro_name(const String& p_file, Size p_line);

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

}  // namespace GodotObjectCompiler

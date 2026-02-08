
#pragma once
#include "library/tree/syntax/context.h"
#include "program.h"

namespace GodotObjectCompiler {

  class Help : public IProgram {
    PROGRAM(Help, "help")

   public:

    Ref<ProgramError> run(ApplicationContext& p_context) override;
  };

  class HelpEntry : public Context {
    NODE_TYPE(HelpEntry)

    HelpEntry(const ProgramPath& path, const String& name, bool is_program)
        : path(path), name(name), is_program(is_program) {}

    String get_help_text() const;

    String to_string() const override;

    ProgramPath path;
    String name;
    bool is_program;
  };

}  // namespace GodotObjectCompiler

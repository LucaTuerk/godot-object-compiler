
#pragma once
#include "application/application_context.h"
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  class ProgramError : public Error {
    NODE_TYPE(ProgramError)

    ProgramError(ErrorLevel level, const String& message) : Error(level, message) {}

    static inline const Ref<ProgramError> OK = nullptr;
  };

  class IProgram {
   public:

    virtual ~IProgram() = default;
    [[nodiscard]] virtual String get_type() const = 0;
    [[nodiscard]] virtual String program_name() const = 0;
    virtual Ref<ProgramError> run(ApplicationContext& context) = 0;
  };

  using ProgramPath = Vector<String>;

  class Programs {
   public:

    static Programs* instance() {
      static Programs instance;
      return &instance;
    }

    bool register_program(const Ref<IProgram>& program);

    Ref<IProgram> find_program(const Vector<String>& application_arguments, Vector<String>& program_arguments);

    const Dictionary<ProgramPath, Ref<IProgram>>& get_programs();

   private:

    static Size overlap(const ProgramPath& a, const ProgramPath& b);

    HashSet<String> _registered_programs;
    Dictionary<ProgramPath, Ref<IProgram>> _programs;
  };

}  // namespace GodotObjectCompiler

#define PROGRAM(type, name)                                     \
 public:                                                        \
                                                                \
  static String get_type_static() { return #type; }             \
  virtual String get_type() const override { return #type; }    \
  virtual String program_name() const override { return name; } \
                                                                \
 private:                                                       \
                                                                \
  static inline bool __program_registered_##type =              \
      Programs::instance()->register_program(std::dynamic_pointer_cast<IProgram>(make_ref<type>()));

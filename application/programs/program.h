
#pragma once
#include "application/application_context.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class IProgram {
   public:

    virtual String program_name() const = 0;
    virtual int run(ApplicationContext& context) = 0;
  };

}  // namespace GodotObjectCompiler

#define PROGRAM(name) \
  virtual String program_name() const override { return name; }

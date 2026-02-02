
#pragma once
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class ApplicationContext {
   public:

    String paths_root;
    String paths_cache;
    String paths_generated;
    Vector<String> files_input;
    Vector<String> paths_include;
    Vector<String> application_arguments;
    Vector<String> program_arguments;

    bool valid() const;
  };

}  // namespace GodotObjectCompiler

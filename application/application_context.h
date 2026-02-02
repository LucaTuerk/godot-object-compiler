
#pragma once
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class ApplicationContext {
   public:

    Vector<String> application_arguments;

    String paths_root;
    String paths_cache;
    String paths_generated;
    Vector<String> files_input;
    Vector<String> paths_include;

    bool valid() const;
  };

}  // namespace GodotObjectCompiler

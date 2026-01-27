
#pragma once
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class ApplicationContext {
   public:

    Vector<String> application_arguments;

    String cache_root;
    String generated_root;
    Vector<String> input_files;
    Vector<String> include_paths;
  };

}  // namespace GodotObjectCompiler

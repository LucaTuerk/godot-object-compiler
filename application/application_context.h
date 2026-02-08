
#pragma once
#include "library/core/core.h"
#include "project.h"

namespace GodotObjectCompiler {

  class ApplicationContext {
   public:

    String paths_root;
    String paths_goc;
    String paths_cache;
    String paths_generated;
    Vector<String> files_input;
    Vector<String> paths_include;
    Vector<String> application_arguments;
    Vector<String> program_arguments;

    bool set_from_project(const Project& p_project);
    bool validate() const;
  };

}  // namespace GodotObjectCompiler

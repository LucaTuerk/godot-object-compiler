#pragma once
#include "../../library/core/core.h"

namespace GodotObjectCompiler {

  namespace ProgramFunctions {
    bool copy_resources_to_folder(const Vector<String>& resource_glob_paths, const String& target_folder);
  };

}  // namespace GodotObjectCompiler


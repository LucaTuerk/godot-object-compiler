
#include "application_context.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  bool ApplicationContext::valid() const {
    bool success = true;

    if (!dir_exists(paths_root)) {
      print_err(format("Invalid root path \"%s\". Path is not a directory.", paths_root.c_str()));
      success = false;
    }

    if (!dir_exists(paths_cache)) {
      print_err(format("Invalid cache path \"%s\". Path is not a directory.", paths_cache.c_str()));
      success = false;
    }

    if (!dir_exists(paths_generated)) {
      print_err(format("Invalid generated path \"%s\". Path is not a directory.", paths_generated.c_str()));
      success = false;
    }

    for (const String& file : files_input) {
      if (!file_exists(file)) {
        print_err(format("Invalid input file \"%s\". File does not exist.", file.c_str()));
        success = false;
      }
    }

    for (const String& include_path : paths_include) {
      if (!dir_exists(include_path)) {
        print_err(format("Invalid include path\"%s\". Path is not a directory.", include_path.c_str()));
      }
    }

    return success;
  }

}  // namespace GodotObjectCompiler

#include "application_context.h"

#include "library/core/helpers.h"
#include "library/core/permissions.h"

namespace GodotObjectCompiler {

  bool ApplicationContext::set_from_project(const Project& p_project) {
    paths_root = path_absolute(p_project.paths_root);
    paths_generated = path_absolute(p_project.paths_generated);
    paths_cache = path_absolute(p_project.paths_cache);
    paths_goc = path_absolute(p_project.paths_goc);

    paths_include = p_project.paths_include;
    if (!vector_contains(paths_include, p_project.paths_root)) {
      paths_include.push_back(p_project.paths_root);
    }

    files_input = directory_files_recursive(p_project.paths_root);

    std::transform(paths_include.begin(), paths_include.end(), paths_include.begin(), &path_absolute);
    std::transform(files_input.begin(), files_input.end(), files_input.begin(), &path_absolute);
    return validate();
  }

  bool ApplicationContext::validate() const {
    bool success = true;

    if (!directory_exits(paths_root)) {
      print_err(format("Invalid root path \"%s\". Path is not a directory.", paths_root.c_str()));
      success = false;
    }

    if (!directory_exits(paths_cache)) {
      print_err(format("Invalid cache path \"%s\". Path is not a directory.", paths_cache.c_str()));
      success = false;
    }

    if (!directory_exits(paths_generated)) {
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
      if (!directory_exits(include_path)) {
        print_err(format("Invalid include path\"%s\". Path is not a directory.", include_path.c_str()));
      }
    }

    if (success) {
      Permissions::instance()->clear();
      Permissions::instance()->add_write_path(paths_goc);
      Permissions::instance()->add_write_path(paths_generated);
      Permissions::instance()->add_write_path(paths_cache);
    }

    return success;
  }

}  // namespace GodotObjectCompiler
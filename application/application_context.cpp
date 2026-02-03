
#include "application_context.h"

#include "library/core/helpers.h"
#include "library/core/permissions.h"

namespace GodotObjectCompiler {

  bool ApplicationContext::set_from_project(const Project& project) {
    paths_root = path_absolute(project.paths_root);
    paths_generated = path_absolute(project.paths_generated);
    paths_cache = path_absolute(project.paths_cache);
    paths_goc = path_absolute(project.paths_goc);

    paths_include = project.paths_include;
    if (std::find(paths_include.begin(), paths_include.end(), project.paths_root) == paths_include.end()) {
      paths_include.push_back(project.paths_root);
    }

    files_input = directory_files_recursive(project.paths_root);

    std::transform(paths_include.begin(), paths_include.end(), paths_include.begin(), &path_absolute);
    std::transform(files_input.begin(), files_input.end(), files_input.begin(), &path_absolute);
    return validate();
  }

  bool ApplicationContext::validate() const {
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

    if (success) {
      Permissions::instance()->clear();
      Permissions::instance()->add_write_path(paths_goc);
      Permissions::instance()->add_write_path(paths_generated);
      Permissions::instance()->add_write_path(paths_cache);
    }

    return success;
  }

}  // namespace GodotObjectCompiler
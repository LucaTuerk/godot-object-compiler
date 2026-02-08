//
// Created by luca on 03.02.26.
//

#include "program_functions.h"

#include "library/core/helpers.h"
#include "library/core/resources.h"

namespace GodotObjectCompiler {

  bool ProgramFunctions::copy_resources_to_folder(
      const Vector<String>& p_resource_glob_paths, const String& p_target_folder) {
    for (const String& copy_resources : p_resource_glob_paths) {
      for (const String& res_path : Resources::instance()->resources_recursive(copy_resources)) {
        String relative = path_relative(res_path, copy_resources);
        auto file_path =
            path_concat(path_concat(p_target_folder, string_replace(copy_resources, "res://", "")), relative);
        if (!file_exists(file_path)) {
          String folder_path = path_base(file_path);
          if (!directory_exits(folder_path) && !create_dir_recursive(folder_path)) {
            return false;
          }
          write_file(file_path, Resources::instance()->load_text_resource(res_path));
        }
      }
    }
    return true;
  }

}  // namespace GodotObjectCompiler
/**************************************************************************/
/* init_local_resources.cpp                                               */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "init_local_resources.h"

#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler {

bool copy_resources_to_folder(const Vector<String>& p_resource_glob_paths,
                              const String& p_target_folder) {
  for (const String& copy_resources : p_resource_glob_paths) {
    for (const String& res_path :
         Resources::instance()->resources_recursive(copy_resources)) {
      String relative = path_relative(res_path, copy_resources);
      auto file_path =
          path_concat(path_concat(p_target_folder,
                                  string_replace(copy_resources, "res://", "")),
                      relative);
      if (!file_exists(file_path)) {
        String folder_path = path_base(file_path);
        if (!directory_exits(folder_path) &&
            !create_dir_recursive(folder_path)) {
          return false;
        }
        write_file(file_path,
                   Resources::instance()->load_text_resource(res_path));
      }
    }
  }
  return true;
}

Ref<ProgramError> InitLocalResources::run(ApplicationContext& p_context) {
  if (!copy_resources_to_folder(
          {
              "res://variant_types",
              "res://macros",
          },
          p_context.paths_goc)) {
    PROG_ERR("Failed to copy some local resources.");
  }
  return ProgramError::OK;
}

}  // namespace GodotObjectCompiler
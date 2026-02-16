/**************************************************************************/
/* application_context.cpp                                                */
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

#include "application_context.h"

#include <complex>

#include "library/core/collection_utilities.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler {

  bool ApplicationContext::set_from_project(const Project& p_project) {
    return true;
    // project_name = p_project.project_name;
    // project_target = p_project.project_target;
    // paths_root = path_absolute(p_project.paths_root);
    // paths_generated = path_absolute(p_project.paths_generated);
    // paths_cache = path_absolute(p_project.paths_cache);
    // paths_readonly_cache = path_concat(paths_cache, ".readonly");
    // paths_goc = path_absolute(p_project.paths_goc);
    //
    // paths_include = p_project.paths_include;
    // for (const auto& godot_include_path : p_project.godot_include_paths) {
    //   if (!vector_contains(paths_include, godot_include_path)) {
    //     paths_include.push_back(godot_include_path);
    //   }
    // }
    //
    // if (!vector_contains(paths_include, p_project.paths_root)) {
    //   paths_include.push_back(p_project.paths_root);
    // }
    //
    // for (const String& file_path : directory_files_recursive(p_project.paths_root)) {
    //   if (string_suffix(file_path, ".h") || string_suffix(file_path, ".hpp")) {
    //     files_input.push_back(file_path);
    //   }
    // }
    //
    // std::transform(paths_include.begin(), paths_include.end(), paths_include.begin(), &path_absolute);
    // std::transform(files_input.begin(), files_input.end(), files_input.begin(), &path_absolute);
    // return validate();
  }

  bool ApplicationContext::set_from_application_arguments(Vector<String>& p_application_arguments) {
    auto prefix_extract = [](const String& p_str, const String& p_long, const String& p_short, String& r_out) {
      if (string_prefix(p_str, p_long)) {
        r_out = p_str.substr(p_long.size());
        return true;
      } else if (string_prefix(p_str, p_short)) {
        r_out = p_str.substr(p_short.size());
        return true;
      }
      r_out = "";
      return false;
    };

    for (auto itr = p_application_arguments.begin(); itr != p_application_arguments.end();) {
      String content;
      String arg = *itr;

      if (prefix_extract(arg, "--root_path=", "-R=", content)) {
        paths_root = content;
        itr = p_application_arguments.erase(itr);
      } else if (prefix_extract(arg, "--goc_path=", "-P=", content)) {
        paths_goc = content;
        itr = p_application_arguments.erase(itr);
      } else if (prefix_extract(arg, "--generated_path=", "-G=", content)) {
        paths_generated = content;
        itr = p_application_arguments.erase(itr);
      } else if (prefix_extract(arg, "--cache_path", "-C=", content)) {
        paths_cache = content;
        itr = p_application_arguments.erase(itr);
      } else if (prefix_extract(arg, "--include_paths", "-I=", content)) {
        paths_include = string_split(content, ",");
        itr = p_application_arguments.erase(itr);
      } else if (prefix_extract(arg, "--sources=", "-S=", content)) {
        files_input = string_split(content, ",");
        itr = p_application_arguments.erase(itr);
      } else {
        ++itr;
      }
    }

    paths_goc = path_absolute(paths_goc);
    paths_cache = path_absolute(paths_cache);
    paths_readonly_cache = path_absolute(paths_readonly_cache);
    paths_generated = path_absolute(paths_generated);

    if (files_input.has_value()) {
      std::transform(files_input->begin(), files_input->end(), files_input->begin(), &path_absolute);
    }

    if (paths_include.has_value()) {
      std::transform(paths_include->begin(), paths_include->end(), paths_include->begin(), &path_absolute);
    }

    return validate();
  }

  bool ApplicationContext::validate() const {
    bool success = true;
    Permissions::instance()->clear();
    Permissions::instance()->add_write_path(paths_goc);
    Permissions::instance()->add_write_path(paths_generated);
    Permissions::instance()->add_write_path(paths_cache);

    if (paths_root.has_value() && !directory_exits(*paths_root)) {
      if (!create_dir_recursive(*paths_root)) {
        print_err(format("Invalid root path \"%s\". Could not find or create directory.", paths_root->c_str()));
        success = false;
      }
    }

    if (!directory_exits(paths_cache)) {
      if (!create_dir_recursive(paths_cache)) {
        print_err(format("Invalid cache path \"%s\". Could not find or create directory.", paths_cache.c_str()));
        success = false;
      }
    }

    if (!directory_exits(paths_generated)) {
      if (!create_dir_recursive(paths_generated)) {
        print_err(
            format("Invalid generated path \"%s\". Could not find or create directory.", paths_generated.c_str()));
        success = false;
      }
    }

    if (files_input.has_value()) {
      for (const String& file : *files_input) {
        if (!file_exists(file)) {
          print_err(format("Invalid input file \"%s\". File does not exist.", file.c_str()));
          success = false;
        }
      }
    }

    if (paths_include.has_value()) {
      for (const String& include_path : *paths_include) {
        if (!directory_exits(include_path)) {
          print_err(format("Invalid include path\"%s\". Path is not a directory.", include_path.c_str()));
        }
      }
    }

    return success;
  }

}

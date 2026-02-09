/**************************************************************************/
/* project.cpp                                                            */
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
//
// Created by luca on 02.02.26.
//

#include "project.h"

#include "library/core/config.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  void Project::read_from(IStructuredReader* p_reader) {
    p_reader->read_from_section("Project");
    project_name = p_reader->read<String, String>("Name");
    String project_target_str = string_trim(p_reader->read<String, String>("Target"));
    project_target = project_target_str == "Module" ? TARGET_MODULE
        : project_target_str == "GDExtension"       ? TARGET_GDEXTENSION
                                                    : TARGET_UNDEFINED;

    p_reader->read_from_section("Godot");
    godot_include_paths = from_comma_separated_string(p_reader->read<String, String>("IncludePaths"));
    godot_target_major_version = p_reader->read<String, Size>("TargetMajorVersion");
    godot_target_minor_version = p_reader->read<String, Size>("TargetMinorVersion");

    p_reader->read_from_section("Paths");
    paths_root = p_reader->read<String, String>("RootPath");
    paths_goc = p_reader->read<String, String>("GOCPath");
    paths_generated = p_reader->read<String, String>("GeneratedPath");
    paths_cache = p_reader->read<String, String>("CachePath");
    paths_include = from_comma_separated_string(p_reader->read<String, String>("IncludePaths"));
    paths_ignore = from_comma_separated_string(p_reader->read<String, String>("IgnorePaths"));
  }

  void Project::write_to(IStructuredWriter* p_writer) {
    p_writer->write_to_section("Project");
    p_writer->write<String, String>("Name", project_name);
    p_writer->write<String, String>("Target",
        project_target == TARGET_MODULE            ? "Module"
            : project_target == TARGET_GDEXTENSION ? "GDExtension"
                                                   : "");

    p_writer->write_to_section("Godot");
    p_writer->write<String, String>("IncludePaths", to_comma_separated_string(godot_include_paths));
    p_writer->write<String, Size>("TargetMajorVersion", godot_target_major_version);
    p_writer->write<String, Size>("TargetMinorVersion", godot_target_minor_version);

    p_writer->write_to_section("Paths");
    p_writer->write<String, String>("RootPath", paths_root);
    p_writer->write<String, String>("GeneratedPath", paths_generated);
    p_writer->write<String, String>("CachePath", paths_cache);
    p_writer->write<String, String>("GOCPath", paths_goc);
    p_writer->write<String, String>("IncludePaths", to_comma_separated_string(paths_include));
    p_writer->write<String, String>("IgnorePaths", to_comma_separated_string(paths_ignore));
  }

  bool Project::read_from_file(const String& p_path) {
    Config config;
    if (!config.read_from_file(p_path)) {
      return false;
    }

    read_from(&config);
    return true;
  }

  bool Project::write_to_file(const String& p_path) {
    Config config;
    write_to(&config);
    return config.write_to_file(p_path);
  }

  Vector<String> Project::from_comma_separated_string(const String& p_str) {
    Vector<String> result = string_split(p_str, ",");
    std::transform(result.begin(), result.end(), result.begin(), &string_trim);
    for (auto itr = result.begin(); itr != result.end();) {
      if (itr->empty()) {
        itr = result.erase(itr);
      } else {
        ++itr;
      }
    }

    return result;
  }

  String Project::to_comma_separated_string(const Vector<String>& p_str) {
    StreamWriter writer;
    for (Size i = 0; i < p_str.size(); i++) {
      if (i != 0 && i != p_str.size() - 1) {
        writer.write(",");
      }
      writer.write(string_trim(p_str[i]));
    }
    return writer.get_string();
  }

}
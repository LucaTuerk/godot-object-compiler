/**************************************************************************/
/* file_system_utilities.cpp                                              */
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

#include "file_system_utilities.h"

#include <filesystem>

#include "core.h"
#include "library/core/string_utilities.h"
#include "library/execution_context.h"
#include "permissions.h"
#include "string_writer.h"

namespace GodotObjectCompiler {

  String read_file(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PRINT_VERBOSE("Reading file \"%s\"", absolute.c_str());
    PANIC_COND(!file_exists(absolute), "Trying to read non-existing file \"%s\"", absolute.c_str());
    std::ifstream ifs;
    ifs.open(absolute);
    PANIC_COND(!ifs.is_open() || ifs.bad() || ifs.fail(), "Failed to open file \"%s\"", absolute.c_str());
    std::string str(std::istreambuf_iterator{ifs}, {});
    return str;
  }

  void write_file(const String& p_path, const String& p_content) {
    const String absolute = path_absolute(p_path);
    PRINT_VERBOSE("Writing file \"%s\"", absolute.c_str());
    Permissions::instance()->ensure_is_allowed_write_path(absolute);

    std::ofstream ofs(absolute.c_str(), std::ios::out | std::ios::binary);
    ofs.write(p_content.c_str(), static_cast<long>(p_content.size()));
  }

  bool file_exists(const String& p_path) {
    const String absolute = path_absolute(p_path);
    return std::filesystem::exists(absolute);
  }

  bool remove_file(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PRINT_VERBOSE("Deleting file \"%s\"", absolute.c_str());
    Permissions::instance()->ensure_is_allowed_write_path(absolute);
    return std::filesystem::remove(absolute.c_str()) == 0;
  }

  bool remove(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PRINT_VERBOSE("Deleting \"%s\"", absolute.c_str());
    Permissions::instance()->ensure_is_allowed_write_path(absolute);
    return std::filesystem::remove_all(absolute.c_str()) > 0;
  }

  void write_initial_file_content(const String& p_path, const String& p_initial_content) {
    const String absolute = path_absolute(p_path);
    if (file_exists(absolute)) {
      return;
    }
    FileWriter writer(absolute);
    writer.write(p_initial_content);
  }

  bool directory_exits(const String& p_path) {
    const String absolute = path_absolute(p_path);
    return std::filesystem::exists(absolute) && std::filesystem::is_directory(absolute);
  }

  bool create_dir_recursive(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PRINT_VERBOSE("Creating directories \"%s\"", absolute.c_str());
    Permissions::instance()->ensure_is_allowed_write_path(absolute);
    return std::filesystem::create_directories(absolute);
  }

  Size file_write_time(const String& p_path) {
    const String absolute = path_absolute(p_path);
    if (!file_exists(absolute)) {
      return 0;
    }

    auto file_time = std::filesystem::last_write_time(absolute);
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(sctp);
  }

  String path_base(const String& p_path) { return std::filesystem::path(p_path).parent_path().generic_string(); }

  String path_concat(const String& p_left, const String& p_right) {
    if (string_prefix(p_left, "res://")) {
      return "res://" + path_concat(p_left.substr(6), p_right);
    }

    return (std::filesystem::path(p_left) / std::filesystem::path(p_right)).generic_string();
  }

  String path_concat_ext(const String& p_dir, const String& p_filename, const String& p_extension) {
    if (string_prefix(p_dir, "res://")) {
      return "res://" + path_concat_ext(p_dir.substr(6), p_filename, p_extension);
    }

    return (
        std::filesystem::path(p_dir) / std::filesystem::path(format("%s.%s", p_filename.c_str(), p_extension.c_str())))
        .generic_string();
  }

  String path_relative(const String& p_path, const String& p_base) {
    const bool path_is_res = string_prefix(p_path, "res://");
    const bool base_is_res = string_prefix(p_base, "res://");
    if (path_is_res != base_is_res) {
      PANIC(
          "Invalid argument. Trying to get relative path but one path is a resource path while the other is a regular "
          "path. (\"%s\", \"%s\")",
          p_path.c_str(), p_base.c_str());
    }

    if (path_is_res && base_is_res) {
      return path_relative(p_path.substr(6), p_base.substr(6));
    }

    return std::filesystem::relative(p_path, p_base).generic_string();
  }

  String path_absolute(const String& p_path) {
    if (p_path.empty()) {
      return path_cwd();
    }
    return std::filesystem::absolute(p_path).generic_string();
  }

  String path_file_name(const String& p_path) {
    std::filesystem::path path = p_path;
    if (!is_regular_file(path)) {
      return "";
    }

    return path.filename();
  }

  String path_cwd() { return std::filesystem::current_path(); }

  String path_stem(const String& p_path) { return std::filesystem::path(p_path).stem().generic_string(); }

  Vector<String> directory_files(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PANIC_COND(!directory_exits(absolute), "Trying to iterate non existing directory \"%s\"", absolute.c_str());

    Vector<String> result;
    std::filesystem::directory_iterator iter(absolute);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }

    return result;
  }

  Vector<String> directory_files_recursive(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PANIC_COND(!directory_exits(absolute), "Trying to iterate non existing directory \"%s\"", absolute.c_str());

    Vector<String> result;
    std::filesystem::recursive_directory_iterator iter(absolute);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }

    return result;
  }

  Vector<String> directory_dirs(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PANIC_COND(!directory_exits(absolute), "Trying to iterate non existing directory \"%s\"", absolute.c_str());

    Vector<String> result;
    std::filesystem::directory_iterator iter(absolute);
    for (const auto& entry : iter) {
      if (entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  Vector<String> directory_entries(const String& p_path) {
    const String absolute = path_absolute(p_path);
    PANIC_COND(!directory_exits(absolute), "Trying to iterate non existing directory \"%s\"", absolute.c_str());

    Vector<String> result;
    std::filesystem::directory_iterator iter(absolute);
    for (const auto& entry : iter) {
      if (entry.is_regular_file() || entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  bool path_is_descendant(const String& p_possible_ancestor, const String& p_possible_child) {
    String ancestor_absolute = path_absolute(p_possible_ancestor);
    String child_absolute = path_absolute(p_possible_child);
    return string_prefix(child_absolute, ancestor_absolute);
  }

  bool copy_file(const String& p_source, const String& p_destination) {
    auto source = path_absolute(p_source);
    auto destination = path_absolute(p_destination);
    PRINT_VERBOSE("Copying file \"%s\" to \"%s\"", source.c_str(), destination.c_str());
    PANIC_COND(source.empty(), "Empty source file path on file copy.");
    PANIC_COND(!file_exists(source), "Trying to copy non-existing file \"%s\"", source.c_str());
    PANIC_COND(destination.empty(), "Empty destination file path on file copy.");
    String destination_base = path_base(destination);
    PANIC_COND(!directory_exits(destination_base) && !create_dir_recursive(destination_base),
        "Target directory \"%s\" does not exist and could not be created on file copy", destination_base.c_str());

    Permissions::instance()->ensure_is_allowed_write_path(destination);
    return std::filesystem::copy_file(source, destination, std::filesystem::copy_options::update_existing);
  }

}

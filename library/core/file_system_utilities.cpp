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
#include "permissions.h"
#include "string_writer.h"

namespace GodotObjectCompiler {

  String read_file(const String& p_path) {
    std::ifstream ifs;
    // prepare f to throw if failbit gets set
    std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
    ifs.exceptions(exceptionMask);

    try {
      ifs.open(p_path);
    } catch (std::ios_base::failure& e) {
      std::cerr << p_path << ": " << e.what() << '\n';
      return "";
    }

    std::string str(std::istreambuf_iterator<char>{ifs}, {});
    return str;
  }

  void write_file(const String& p_path, const String& p_content) {
    Permissions::instance()->ensure_is_allowed_write_path(p_path);
    std::ofstream ofs(p_path.c_str(), std::ios::out | std::ios::binary);
    ofs.write(p_content.c_str(), p_content.size());
  }

  bool file_exists(const String& p_path) { return std::filesystem::exists(p_path); }

  bool remove_file(const String& p_path) {
    Permissions::instance()->ensure_is_allowed_write_path(p_path);
    return std::filesystem::remove(p_path.c_str()) == 0;
  }

  bool remove(const String& p_path) {
    Permissions::instance()->ensure_is_allowed_write_path(p_path);
    return std::filesystem::remove_all(p_path.c_str()) > 0;
  }

  void ensure_file_exists(const String& p_path, const String& p_initial_content) {
    if (file_exists(p_path)) {
      return;
    }

    try {
      FileWriter writer(p_path);
      writer.write(p_initial_content);
    } catch (std::exception& e) {
      PANIC("Failed to write file: %s", e.what());
    }
  }

  bool directory_exits(const String& p_path) {
    return std::filesystem::exists(p_path) && std::filesystem::is_directory(p_path);
  }

  bool create_dir_recursive(const String& p_path) {
    Permissions::instance()->ensure_is_allowed_write_path(p_path);
    return std::filesystem::create_directories(p_path);
  }

  Size file_write_time(const String& p_path) {
    auto file_time = std::filesystem::last_write_time(p_path);
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(sctp);
  }

  String path_base(const String& p_path) { return std::filesystem::path(p_path).parent_path().generic_string(); }

  String path_concat(const String& p_left, const String& p_right) {
    return (std::filesystem::path(p_left) / std::filesystem::path(p_right)).generic_string();
  }

  String path_concat_ext(const String& p_dir, const String& p_filename, const String& p_extension) {
    return (
        std::filesystem::path(p_dir) / std::filesystem::path(format("%s.%s", p_filename.c_str(), p_extension.c_str())))
        .generic_string();
  }

  String path_relative(const String& p_path, const String& p_base) {
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
    Vector<String> result;
    std::filesystem::directory_iterator iter(p_path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  Vector<String> directory_files_recursive(const String& p_path) {
    if (!std::filesystem::is_directory(p_path)) {
      fmt_print_err(p_path + " is not a directory!");
      return {};
    }

    Vector<String> result;
    std::filesystem::recursive_directory_iterator iter(p_path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }

    return result;
  }

  Vector<String> directory_dirs(const String& p_path) {
    Vector<String> result;
    std::filesystem::directory_iterator iter(p_path);
    for (const auto& entry : iter) {
      if (entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  Vector<String> directory_entries(const String& p_path) {
    Vector<String> result;
    std::filesystem::directory_iterator iter(p_path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file() || entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

}
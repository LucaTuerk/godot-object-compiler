/**************************************************************************/
/* helpers.cpp                                                            */
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
#include "helpers.h"

#include <filesystem>
#include <iostream>
#include <random>

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

  Vector<String> read_lines(const String& p_path) {
    Vector<String> result;
    std::ifstream ifs{p_path};

    for (std::string line; std::getline(ifs, line);) {
      result.emplace_back(line);
    }

    return result;
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
      print_err(p_path + " is not a directory!");
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

  String hash_string(Hash p_hash) {
    std::stringstream strstr;
    strstr << p_hash;
    return strstr.str();
  }

  String generate_random_string(size_t p_length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < p_length; ++i) {
      random_string += characters[distribution(generator)];
    }

    return random_string;
  }

  bool string_contains(const String& p_content, const String& p_check) {
    return p_content.find(p_check) != String::npos;
  }

  bool string_suffix(const String& p_content, const String& p_suffix) {
    return p_content.rfind(p_suffix) == p_content.size() - p_suffix.size();
  }

  bool string_prefix(const String& p_content, const String& p_prefix) { return p_content.find(p_prefix) == 0; }

  bool string_only_contains(const String& p_content, char p_char) {
    if (p_content.length() == 0) {
      return false;
    }

    for (char c : p_content) {
      if (c != p_char) {
        return false;
      }
    }

    return true;
  }

  String string_vector_combine(const Vector<String>& p_vector, String p_delimiter) {
    StreamWriter writer;
    for (Size i = 0; i < p_vector.size(); i++) {
      if (i != 0) {
        writer.write(p_delimiter);
      }
      writer.write(p_vector.at(i));
    }
    return writer.get_string();
  }

  bool is_whitespace(char p_char) { return std::isspace(static_cast<unsigned char>(p_char)); }

  String input(const String& p_prompt, const String& p_default_value) {
    String result;
    if (p_default_value.empty()) {
      std::cout << p_prompt;
    } else {
      std::cout << format(p_prompt, p_default_value.c_str());
    }
    getline(std::cin, result);

    if (result.empty()) {
      return p_default_value;
    }
    return result;
  }

  String string_replace(const String& p_target, const String& p_search_str, const String& p_replace_with) {
    std::stringstream strstr;

    Size length = p_search_str.length();
    Size start = 0;
    Size end = p_target.find(p_search_str);

    while (end != String::npos) {
      strstr << p_target.substr(start, end - start);
      strstr << p_replace_with;
      start = end + length;
      end = p_target.find(p_search_str, start);
    }

    strstr << p_target.substr(start);
    return strstr.str();
  }

  String extract_lines(const String& p_content, Size p_start_line, Size p_end_line, Size p_highlight_line) {
    std::stringstream cntstr(p_content);
    std::stringstream trgstr;

    String line;
    Size current = 0;
    while (std::getline(cntstr, line)) {
      current++;
      if (current >= p_start_line && current <= p_end_line) {
        trgstr << current << (current == p_highlight_line ? "\t|>\t" : "\t|\t") << line << '\n';
      }

      if (current >= p_end_line) {
        break;
      }
    }

    return trgstr.str();
  }

  String string_trim(const String& p_content) { return string_trim_left(string_trim_right(p_content)); }

  String string_trim_left(const String& p_content) {
    String ret = p_content;
    auto itr = std::find_if(ret.begin(), ret.end(), [](unsigned char c) { return !is_whitespace(c); });
    ret.erase(ret.begin(), itr);
    return ret;
  }

  String string_trim_right(const String& p_content) {
    String ret = p_content;
    auto itr = std::find_if(ret.rbegin(), ret.rend(), [](unsigned char c) { return !is_whitespace(c); });
    ret.erase(itr.base(), ret.end());
    return ret;
  }

  String string_pad_right(const String& p_content, char p_padding, Size p_size) {
    if (p_content.size() >= p_size) {
      return p_content;
    }

    StreamWriter writer;
    writer.write(p_content);
    for (Size i = p_content.length(); i < p_size; i++) {
      writer.write_generic(p_padding);
    }

    return writer.get_string();
  }

  String string_shrink_inner_space(const String& p_content) {
    StreamWriter writer;
    Size whitespace_count = 0;
    for (char c : p_content) {
      if (!is_whitespace(c)) {
        if (whitespace_count > 0) {
          writer.write(" ");
          whitespace_count = 0;
        }
        writer.write_generic(c);
      } else {
        whitespace_count++;
      }
    }
    return writer.get_string();
  }

  int string_to_int(const String& p_content) {
    // if (string_prefix(str, "0x")) {
    //   String substr = str.substr(2);
    //   return std::stoi(substr, nullptr, 8);
    // } else if (string_prefix(str, "0b")) {
    //   String substr = str.substr(2);
    //   return std::stoi(substr, nullptr, 2);
    // } else if (string_prefix(str, "0X")) {
    //   String substr = str.substr(2);
    //   return std::stoi(substr, nullptr, 16);
    // }else {
    return stoi(p_content, nullptr, 0);
    // }
  }

  String macro_case_to_pascal_case(const String& p_content) {
    std::stringstream strstr;

    Size start = 0;
    Size end = String::npos;
    do {
      end = p_content.find("_", start);

      for (Size i = start; i < std::min(end, p_content.length()); ++i) {
        strstr << ((i == start) ? (char)std::toupper(p_content[i]) : (char)std::tolower(p_content[i]));
      }

      start = end + 1;
    } while (end != String::npos);

    return strstr.str();
  }

  String cpp_enum_case_to_exposed_enum_case(const String& p_content) {
    StreamWriter writer;

    enum Type { NONE, SPACE, DIGIT, LETTER };

    Type last = NONE;

    for (char c : p_content) {
      Type current = isdigit(c) ? DIGIT : isalpha(c) ? LETTER : SPACE;

      if (current != last && last != SPACE && last != NONE) {
        writer.write(" ");
      }

      if (current == LETTER) {
        if (current != last) {
          writer.write_generic(static_cast<char>(toupper(c)));
        } else {
          writer.write_generic(static_cast<char>(tolower(c)));
        }
      } else if (current == DIGIT) {
        writer.write_generic(c);
      }

      last = current;
    }
    return writer.get_string();
  }

  Vector<String> string_split(const String& p_content, const String& p_delimiter) {
    Vector<String> result;

    Size start = 0;
    Size end = 0;
    Size length = p_content.length();

    do {
      end = p_content.find(p_delimiter, start);
      result.emplace_back(p_content.substr(start, end - start));
      start = end + p_delimiter.length();
    } while (end < length);

    if (result.empty()) {
      result.emplace_back(p_content);
    }

    return result;
  }

  String format(const String& format_str) { return format_str; }

  void print_ln(const String& str) { std::cout << str << std::endl; }

  void print_err(const String& str) { std::cerr << str << std::endl; }

}  // namespace GodotObjectCompiler
#include "helpers.h"

#include <filesystem>
#include <iostream>
#include <random>

#include "core.h"
#include "permissions.h"
#include "string_writer.h"

namespace GodotObjectCompiler {

  String read_file(const String& path) {
    std::ifstream ifs;
    // prepare f to throw if failbit gets set
    std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
    ifs.exceptions(exceptionMask);

    try {
      ifs.open(path);
    } catch (std::ios_base::failure& e) {
      std::cerr << path << ": " << e.what() << '\n';
      return "";
    }

    std::string str(std::istreambuf_iterator<char>{ifs}, {});
    return str;
  }

  Vector<String> read_lines(const String& path) {
    Vector<String> result;
    std::ifstream ifs{path};

    for (std::string line; std::getline(ifs, line);) {
      result.emplace_back(line);
    }

    return result;
  }

  void write_file(const String& path, const String& content) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    std::ofstream ofs(path.c_str(), std::ios::out | std::ios::binary);
    ofs.write(content.c_str(), content.size());
  }

  bool file_exists(const String& path) { return std::filesystem::exists(path); }

  bool remove_file(const String& path) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    return std::filesystem::remove(path.c_str()) == 0;
  }

  bool remove(const String& path) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    return std::filesystem::remove_all(path.c_str()) > 0;
  }

  void ensure_file_exists(const String& path, const String& initial_content) {
    if (file_exists(path)) {
      return;
    }

    try {
      FileWriter writer(path);
      writer.write(initial_content);
    } catch (std::exception& e) {
      PANIC("Failed to write file: %s", e.what());
    }
  }

  bool dir_exists(const String& path) { return std::filesystem::exists(path) && std::filesystem::is_directory(path); }

  bool create_dir_recursive(const String& path) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    return std::filesystem::create_directories(path);
  }

  Size file_write_time(const String& path) {
    auto file_time = std::filesystem::last_write_time(path);
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(sctp);
  }

  String path_base(const String& path) { return std::filesystem::path(path).parent_path().generic_string(); }

  String path_concat(const String& path1, const String& path2) {
    return (std::filesystem::path(path1) / std::filesystem::path(path2)).generic_string();
  }

  String path_concat_ext(const String& dir, const String& filename, const String& extension) {
    return (std::filesystem::path(dir) / std::filesystem::path(format("%s.%s", filename.c_str(), extension.c_str())))
        .generic_string();
  }

  String path_relative(const String& path, const String& base) {
    return std::filesystem::relative(path, base).generic_string();
  }

  String path_absolute(const String& path) {
    if (path.empty()) {
      return path_cwd();
    }
    return std::filesystem::absolute(path).generic_string();
  }

  String path_cwd() { return std::filesystem::current_path(); }

  String path_stem(const String& path) { return std::filesystem::path(path).stem().generic_string(); }

  Vector<String> directory_files(const String& path) {
    Vector<String> result;
    std::filesystem::directory_iterator iter(path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  Vector<String> directory_files_recursive(const String& path) {
    if (!std::filesystem::is_directory(path)) {
      print_err(path + " is not a directory!");
      return {};
    }

    Vector<String> result;
    std::filesystem::recursive_directory_iterator iter(path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file()) {
        result.push_back(entry.path().string());
      }
    }

    return result;
  }

  Vector<String> directory_dirs(const String& path) {
    Vector<String> result;
    std::filesystem::directory_iterator iter(path);
    for (const auto& entry : iter) {
      if (entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  Vector<String> directory_entries(const String& path) {
    Vector<String> result;
    std::filesystem::directory_iterator iter(path);
    for (const auto& entry : iter) {
      if (entry.is_regular_file() || entry.is_directory()) {
        result.push_back(entry.path().string());
      }
    }
    return result;
  }

  String hash_string(Hash hash) {
    std::stringstream strstr;
    strstr << hash;
    return strstr.str();
  }

  String generate_random_string(size_t length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
      random_string += characters[distribution(generator)];
    }

    return random_string;
  }

  bool string_contains(const String& str, const String& str2) { return str.find(str2) != String::npos; }

  bool string_suffix(const String& str, const String& suffix) {
    return str.rfind(suffix) == str.size() - suffix.size();
  }

  bool string_prefix(const String& str, const String& prefix) { return str.find(prefix) == 0; }

  bool string_only_contains(const String& str, char symbol) {
    if (str.length() == 0) {
      return false;
    }

    for (char c : str) {
      if (c != symbol) {
        return false;
      }
    }

    return true;
  }

  String string_vector_combine(const Vector<String>& vec, String delimiter) {
    StreamWriter writer;
    for (Size i = 0; i < vec.size(); i++) {
      if (i != 0) {
        writer.write(delimiter);
      }
      writer.write(vec.at(i));
    }
    return writer.get_string();
  }

  bool is_whitespace(char c) { return std::isspace(static_cast<unsigned char>(c)); }

  String input(const String& prompt, const String& default_value) {
    String result;
    if (default_value.empty()) {
      std::cout << prompt;
    } else {
      std::cout << format(prompt, default_value.c_str());
    }
    getline(std::cin, result);

    if (result.empty()) {
      return default_value;
    }
    return result;
  }

  String string_replace(const String& target, const String& search_str, const String& replace_with) {
    std::stringstream strstr;

    Size length = search_str.length();
    Size start = 0;
    Size end = target.find(search_str);

    while (end != String::npos) {
      strstr << target.substr(start, end - start);
      strstr << replace_with;
      start = end + length;
      end = target.find(search_str, start);
    }

    strstr << target.substr(start);
    return strstr.str();
  }

  String extract_lines(const String& content, Size start_line, Size end_line, Size highlight_line) {
    std::stringstream cntstr(content);
    std::stringstream trgstr;

    String line;
    Size current = 0;
    while (std::getline(cntstr, line)) {
      current++;
      if (current >= start_line && current <= end_line) {
        trgstr << current << (current == highlight_line ? "\t|>\t" : "\t|\t") << line << '\n';
      }

      if (current >= end_line) {
        break;
      }
    }

    return trgstr.str();
  }

  String string_trim(const String& str) { return string_trim_left(string_trim_right(str)); }

  String string_trim_left(const String& str) {
    String ret = str;
    auto itr = std::find_if(ret.begin(), ret.end(), [](unsigned char c) { return !is_whitespace(c); });
    ret.erase(ret.begin(), itr);
    return ret;
  }

  String string_trim_right(const String& str) {
    String ret = str;
    auto itr = std::find_if(ret.rbegin(), ret.rend(), [](unsigned char c) { return !is_whitespace(c); });
    ret.erase(itr.base(), ret.end());
    return ret;
  }

  String string_shrink_inner_space(const String& str) {
    StreamWriter writer;
    Size whitespace_count = 0;
    for (char c : str) {
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

  int string_to_int(const String& str, const int base) {
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
    return stoi(str, nullptr, 0);
    // }
  }

  String macro_case_to_pascal_case(const String& input) {
    std::stringstream strstr;

    Size start = 0;
    Size end = String::npos;
    do {
      end = input.find("_", start);

      for (Size i = start; i < std::min(end, input.length()); ++i) {
        strstr << ((i == start) ? (char)std::toupper(input[i]) : (char)std::tolower(input[i]));
      }

      start = end + 1;
    } while (end != String::npos);

    return strstr.str();
  }

  String cpp_enum_case_to_exposed_enum_case(const String& input) {
    StreamWriter writer;

    enum Type { NONE, SPACE, DIGIT, LETTER };

    Type last = NONE;

    for (char c : input) {
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

  Vector<String> string_split(const String& str, const String& delimiter) {
    Vector<String> result;

    Size start = 0;
    Size end = 0;
    Size length = str.length();

    do {
      end = str.find(delimiter, start);
      result.emplace_back(str.substr(start, end - start));
      start = end + delimiter.length();
    } while (end < length);

    if (result.empty()) {
      result.emplace_back(str);
    }

    return result;
  }

  String format(const String& format_str) { return format_str; }

  void print_ln(const String& str) { std::cout << str << std::endl; }

  void print_err(const String& str) { std::cerr << str << std::endl; }

}  // namespace GodotObjectCompiler
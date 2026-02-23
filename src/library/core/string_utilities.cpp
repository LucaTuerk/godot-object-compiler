/**************************************************************************/
/* string_utilities.cpp                                                   */
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

#include "string_utilities.h"

#include "core.h"
#include "file_system_utilities.h"
#include "string_writer.h"

namespace GodotObjectCompiler {

  String string_replace(const String& p_target, const String& p_search_str, const String& p_replace_with) {
    std::stringstream strstr;

    const Size length = p_search_str.length();
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
    std::stringstream content_stream(p_content);
    std::stringstream target_stream;

    String line;
    Size current = 0;
    while (std::getline(content_stream, line)) {
      current++;
      if (current >= p_start_line && current <= p_end_line) {
        target_stream << current << (current == p_highlight_line ? "\t|>\t" : "\t|\t") << line << '\n';
      }

      if (current >= p_end_line) {
        break;
      }
    }

    return target_stream.str();
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

  String string_pad_left(const String& p_content, char p_padding, Size p_size) {
    if (p_content.size() >= p_size) {
      return p_content;
    }

    StreamWriter writer;
    for (Size i = p_content.length(); i < p_size; i++) {
      writer.write_generic(p_padding);
    }
    writer.write(p_content);

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
    int result;
    PANIC_COND(!string_to_int(p_content, result), "Failed to convert \"%s\" to int.", p_content.c_str());
    return result;
  }

  bool string_to_int(const String& p_content, int& r_result) {
    char* ptr;

    if(string_prefix(p_content, "0b")) {
      // automatic base resolution fails here in MSVC
      r_result = std::strtol(p_content.substr(2).c_str(), &ptr, 2);
    } else {
      r_result = std::strtol(p_content.c_str(), &ptr, 0);
    }

    return *ptr == 0;
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

  Vector<String> string_split_length(const String& p_content, Size length) {
    if (p_content.empty()) {
      return {""};
    }
    Size current = 0;
    Vector<String> result;
    while (current < p_content.length()) {
      result.push_back(p_content.substr(current, length));
      current += length;
    }

    return result;
  }

  String format(const String& p_format_string) { return p_format_string; }

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

  bool string_enclosed_by(const String& p_content, const String& p_enclosing) {
    return string_prefix(p_content, p_enclosing) && string_suffix(p_content, p_enclosing);
  }

  bool string_only_contains(const String& p_content, char p_char) {
    if (p_content.empty()) {
      return false;
    }

    return std::all_of(p_content.begin(), p_content.end(), [p_char](char c) { return c == p_char; });
    ;
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

  Vector<String> read_lines(const String& p_path) {
    String absolute = path_absolute(p_path);
    PANIC_COND(!file_exists(absolute), "Trying to read non-existing file \"%s\"", absolute.c_str());

    Vector<String> result;
    std::ifstream ifs{p_path};

    for (std::string line; std::getline(ifs, line);) {
      result.emplace_back(line);
    }

    return result;
  }

}

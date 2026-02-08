/**************************************************************************/
/* helpers.h                                                              */
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
#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  String read_file(const String& p_path);

  Vector<String> read_lines(const String& p_path);

  void write_file(const String& p_path, const String& p_content);

  void ensure_file_exists(const String& p_path, const String& p_initial_content);

  bool create_dir_recursive(const String& p_path);

  bool file_exists(const String& p_path);

  bool remove_file(const String& p_path);

  bool remove(const String& p_path);

  bool directory_exits(const String& p_path);

  Size file_write_time(const String& p_path);

  String input(const String& p_prompt, const String& p_default_value = "");

  String path_base(const String& p_path);

  String path_concat(const String& p_left, const String& p_right);

  String path_concat_ext(const String& p_dir, const String& p_filename, const String& p_extension);

  String path_relative(const String& p_path, const String& p_base);

  String path_absolute(const String& p_path);

  String path_file_name(const String& p_path);

  String path_cwd();

  String path_stem(const String& p_path);

  Vector<String> directory_files(const String& p_path);

  Vector<String> directory_files_recursive(const String& p_path);

  Vector<String> directory_dirs(const String& p_path);

  Vector<String> directory_entries(const String& p_path);

  String hash_string(Hash p_hash);

  String generate_random_string(size_t p_length);

  bool is_whitespace(char p_char);

  bool string_contains(const String& p_content, const String& p_check);

  bool string_suffix(const String& p_content, const String& p_suffix);

  bool string_prefix(const String& p_content, const String& p_prefix);

  bool string_only_contains(const String& p_content, char p_char);

  String string_vector_combine(const Vector<String>& p_vector, String p_delimiter);

  String string_replace(const String& p_target, const String& p_search_str, const String& p_replace_with);

  String extract_lines(const String& p_content, Size p_start_line, Size p_end_line, Size p_highlight_line);

  String string_trim(const String& p_content);

  String string_trim_left(const String& p_content);

  String string_trim_right(const String& p_content);

  String string_pad_right(const String& p_content, char p_padding, Size p_size);

  String string_shrink_inner_space(const String& p_content);

  int string_to_int(const String& p_content);

  String macro_case_to_pascal_case(const String& p_content);

  String cpp_enum_case_to_exposed_enum_case(const String& p_content);

  Vector<String> string_split(const String& p_content, const String& p_delimiter);

  template <typename T, typename X>
  bool vector_contains(const Vector<T>& vector, const X& item) {
    return std::find(vector.begin(), vector.end(), T(item)) != vector.end();
  }

  template <typename T>
  typename Vector<T>::iterator find(const Vector<T>& vector, const T& item) {
    return std::find(vector.begin(), vector.end(), item);
  }

  template <typename... Args>
  String format(const String& format_str, Args&&... args);

  template <typename... Args>
  String format(const String& format_str, Args&&... args) {
    const std::size_t n = sizeof...(Args);
    if (n == 0) {
      return format_str;
    }

    int size_s = std::snprintf(nullptr, 0, format_str.c_str(), args...) + 1;
    if (size_s <= 0) {
      return "";
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format_str.c_str(), args...);
    return String(buf.get(), buf.get() + size - 1);
  }

}  // namespace GodotObjectCompiler
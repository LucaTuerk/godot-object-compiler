#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  String read_file(const String& path);
  Vector<String> read_lines(const String& path);
  void write_file(const String& path, const String& content);
  void ensure_file_exists(const String& path, const String& initial_content);
  bool create_dir_recursive(const String& path);
  bool file_exists(const String& path);
  bool remove_file(const String& path);
  bool dir_exists(const String& path);

  Size file_write_time(const String& path);

  String path_base(const String& path);
  String path_concat(const String& path1, const String& path2);
  String path_concat_ext(const String& dir, const String& filename, const String& extension);
  String path_relative(const String& path, const String& base);
  String path_absolute(const String& path);
  String path_stem(const String& path);
  Vector<String> directory_files(const String& path);
  Vector<String> directory_files_recursive(const String& path);
  Vector<String> directory_dirs(const String& path);

  String hash_string(Hash hash);

  String generate_random_string(size_t length);

  bool string_contains(const String& str, const String& str2);
  bool string_suffix(const String& str, const String& suffix);
  bool string_prefix(const String& str, const String& prefix);
  bool string_only_contains(const String& str, char symbol);
  String string_vector_combine(const Vector<String>& vec, String delimiter);
  bool is_whitespace(char c);

  String input(const String& prompt, const String& default_value = "");
  String string_replace(const String& target, const String& search_str, const String& replace_with);
  String extract_lines(const String& content, Size start_line, Size end_line, Size highlight_line);
  String string_trim(const String& str);
  String string_trim_left(const String& str);
  String string_trim_right(const String& str);
  String string_shrink_inner_space(const String& str);
  String macro_case_to_pascal_case(const String& input);
  Vector<String> string_split(const String& str, const String& delimiter);

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

#define PANIC(...)                                                               \
  print_err(format("PANIC!! %s:%d ", __FILE__, __LINE__) + format(__VA_ARGS__)); \
  abort()

#define PANIC_COND(condition, ...) \
  if ((condition)) {               \
    PANIC(__VA_ARGS__);            \
  }

#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  String read_file(const String& path);
  Vector<String> read_lines(const String& path);
  void write_file(const String& path, const String& content);
  bool file_exists(const String& path);
  bool dir_exists(const String& path);
  bool create_dir_recursive(const String& path);

  Size file_write_time(const String& path);

  String path_base(const String& path);
  String path_concat(const String& path1, const String& path2);
  Vector<String> directory_files(const String& path);
  Vector<String> directory_files_recursive(const String& path);
  Vector<String> directory_dirs(const String& path);

  String hash_string(Hash hash);

  String generate_random_string(size_t length);

  bool string_contains(const String& str, const String& str2);
  bool string_suffix(const String& str, const String& str2);
  bool string_only_contains(const String& str, char symbol);
  bool is_whitespace(char c);
  String string_replace(const String& target, const String& search_str, const String& replace_with);
  String extract_lines(const String& content, Size start_line, Size end_line, Size highlight_line);
  String string_trim(const String& str);
  String string_trim_left(const String& str);
  String string_trim_right(const String& str);

  String macro_case_to_pascal_case(const String& input);

  Vector<String> string_split(const String& str, const String& delimiter);

}  // namespace GodotObjectCompiler

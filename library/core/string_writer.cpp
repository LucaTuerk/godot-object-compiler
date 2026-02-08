
#include "string_writer.h"

#include <filesystem>
#include <iostream>

#include "helpers.h"
#include "permissions.h"

namespace GodotObjectCompiler {

  void StreamWriter::write(const String& p_value) {
    _stream << p_value;
    _current_length += p_value.length();
  }

  String StreamWriter::get_string() { return _stream.str(); }

  Size StreamWriter::current_length() { return _current_length; }

  FileWriter::FileWriter(const String& path, bool do_not_write_same_content) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    this->path = path;
    this->do_not_write_same_content = do_not_write_same_content;
    if (!do_not_write_same_content) {
      _file = std::fstream(path, std::ios::out);
    }
  }

  FileWriter::~FileWriter() {
    if (do_not_write_same_content && (!file_exists(path) || read_file(path) != _stream.get_string())) {
      write_file(path, _stream.get_string());
    }
  }

  void FileWriter::write(const String& p_value) {
    if (!do_not_write_same_content) {
      _file << p_value;
    }
    _stream.write(p_value);
  }

  String FileWriter::get_string() { return _stream.get_string(); }

  Size FileWriter::current_length() { return _stream.current_length(); }

}  // namespace GodotObjectCompiler
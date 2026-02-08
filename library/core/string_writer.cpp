/**************************************************************************/
/* string_writer.cpp                                                      */
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

#include "string_writer.h"

#include <filesystem>

#include "file_system_utilities.h"
#include "permissions.h"
#include "resources.h"
#include "string_utilities.h"

namespace GodotObjectCompiler {

  void StreamWriter::write(const String& p_value) {
    _stream << p_value;
    _current_length += p_value.length();
  }

  String StreamWriter::get_string() { return _stream.str(); }

  Size StreamWriter::current_length() { return _current_length; }

  FileWriter::FileWriter(const String& path, bool do_not_write_same_content) {
    Permissions::instance()->ensure_is_allowed_write_path(path);
    this->_path = path;
    this->_do_not_write_same_content = do_not_write_same_content;
    if (!do_not_write_same_content) {
      _file = std::fstream(path, std::ios::out);
    }
  }

  FileWriter::~FileWriter() {
    if (_do_not_write_same_content && (!file_exists(_path) || read_file(_path) != _stream.get_string())) {
      write_file(_path, _stream.get_string());
    }
  }

  FileWriter FileWriter::generated(const String& path) {
    FileWriter writer(path, true);
    writer.write(_generated_header(path_file_name(path)));
    return writer;
  }

  void FileWriter::write(const String& p_value) {
    if (!_do_not_write_same_content) {
      _file << p_value;
    }
    _stream.write(p_value);
  }

  String FileWriter::get_string() { return _stream.get_string(); }

  Size FileWriter::current_length() { return _stream.current_length(); }

  String FileWriter::_generated_header(const String& p_file_name) {
    auto resource_path = "res://generator/generated_header.txt";
    if (!Resources::instance()->has_resource(resource_path)) {
      return "";
    }

    String content = Resources::instance()->load_text_resource(resource_path);
    Size file_name_max = 70;
    String file_name_search_string = string_pad_right("FILENAME", ' ', file_name_max);
    String version_search_string = "GOC_VERSION";
    content = string_replace(content, file_name_search_string, string_pad_right(p_file_name, ' ', file_name_max));
    content =
        string_replace(content, version_search_string, string_pad_right("0.0", ' ', version_search_string.length()));
    return content;
  }

}
/**************************************************************************/
/* reader_writer.h                                                        */
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

class IStringWriter {
 public:
  virtual ~IStringWriter() = default;
  virtual void write(const String& p_value) = 0;

  template <typename T>
  void write_generic(const T& value);

  virtual String get_string() = 0;
  virtual Size current_length() = 0;
};

class IStructuredWriter {
 public:
  virtual ~IStructuredWriter() = default;

  template <typename K, typename V>
  void write(const K& key, const V& value);

  template <typename S>
  void write_to_section(const S& section);

  virtual bool write_to_file(const String& p_path) = 0;

 protected:
  virtual void _write_to_section(const String& p_section) = 0;
  virtual void _write(const String& p_key, const String& p_value) = 0;
};

class IStructuredReader {
 public:
  virtual ~IStructuredReader() = default;

  template <typename K, typename V>
  V read(const K& key);

  template <typename S>
  void read_from_section(const S& section);

  virtual bool read_from_file(const String& p_path) = 0;

 protected:
  virtual void _read_from_section(const String& p_section) = 0;
  virtual String _read(const String& p_key) = 0;
};

template <typename T>
void IStringWriter::write_generic(const T& value) {
  std::stringstream strstr;
  strstr << value;
  write(strstr.str());
}

template <typename K, typename V>
void IStructuredWriter::write(const K& key, const V& value) {
  std::stringstream key_str, value_str;
  key_str << key;
  value_str << value;

  _write(key_str.str(), value_str.str());
}

template <typename S>
void IStructuredWriter::write_to_section(const S& section) {
  std::stringstream section_str;
  section_str << section;

  _write_to_section(section_str.str());
}

template <typename K, typename V>
V IStructuredReader::read(const K& key) {
  std::stringstream key_str;
  key_str << key;

  std::istringstream ss(_read(key_str.str()));
  V result;
  ss >> result;

  return result;
}

template <typename S>
void IStructuredReader::read_from_section(const S& section) {
  std::stringstream section_str;
  section_str << section;

  _read_from_section(section_str.str());
}

class Node;

class INodeWriter {
 public:
  virtual ~INodeWriter() = default;

  virtual bool write_to_file(Ref<Node> node, const String& path) = 0;
};

class INodeReader {
 public:
  virtual ~INodeReader() = default;

  virtual Ref<Node> read_from_file(const String& path) = 0;

  template <typename T>
  Ref<T> read_from_file(const String& p_path);
};

}  // namespace GodotObjectCompiler
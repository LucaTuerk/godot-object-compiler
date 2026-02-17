/**************************************************************************/
/* execution_context.cpp                                                  */
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

#include "execution_context.h"

#include <algorithm>

#include "attribute_db.h"
#include "core/file_system_utilities.h"
#include "core/string_utilities.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "library/node_db.h"
#include "node_db.h"
#include "type_db.h"

namespace GodotObjectCompiler {

  ExecutionContext::ExecutionContext() { init(); }

  String error_level_to_string(ErrorLevel level) {
    switch (level) {
      case ERROR:
        return "Error";
        break;
      case WARNING:
        return "Warning";
        break;
      case INFO:
        return "Info";
        break;
      case VERBOSE:
        return "Verbose";
        break;
    }
    return "";
  }

  void ExecutionContext::init() {
    _node_db = make_ref<NodeDB>(NodeDB::Private());
    _attribute_db = make_ref<AttributeDB>(AttributeDB::Private());
    _type_db = make_ref<TypeDB>(TypeDB::Private());
    _include_paths = {};
    _remove_macros = {};
    _generated_from = {};
  }

  NodeDB* ExecutionContext::get_node_db() { return _node_db.get(); }

  AttributeDB* ExecutionContext::get_attribute_db() { return _attribute_db.get(); }

  TypeDB* ExecutionContext::get_type_db() { return _type_db.get(); }

  const Vector<String>& ExecutionContext::get_remove_macros() { return _remove_macros; }

  void ExecutionContext::set_remove_macros(const Vector<String>& p_value) { _remove_macros = p_value; }

  const Vector<String>& ExecutionContext::get_include_paths() { return _include_paths; }

  void ExecutionContext::set_include_paths(const Vector<String>& p_value) { _include_paths = p_value; }

  void ExecutionContext::set_error_level(ErrorLevel p_level, ErrorDetail p_error_detail) {
    _error_level = p_level;
    _error_detail = p_error_detail;
  }

  ErrorLevel ExecutionContext::get_error_level() const { return _error_level; }

  ErrorDetail ExecutionContext::get_error_detail() const { return _error_detail; }

  void ExecutionContext::print(ErrorLevel p_level, const String& p_message) const {
    if (p_level >= _error_level) {
      print_ln(p_message);
    }
  }

  Hash ExecutionContext::get_path_hash(const String& p_absolute_path) {
    Hasher<String> hasher;
    return hasher(p_absolute_path);
  }

  void ExecutionContext::register_generated_file(const String& p_generated_path, const String& p_generated_from_path) {
    _generated_from[p_generated_from_path].push_back(p_generated_path);
  }

  bool ExecutionContext::load_generated_from_file(const String& p_path) {
    Config config;
    if (!config.read_from_file(p_path)) {
      return false;
    }

    _generated_from.clear();

    for (const String& key : config.get_sections()) {
      config.read_from_section(key);
      if (config.has_config_value("generated_files")) {
        Vector<String> generated = string_split(config.read<String, String>("generated_files"), ";");
        _generated_from.emplace(key, generated);
      }
    }

    return true;
  }

  bool ExecutionContext::save_generated_from_file(const String& p_path) {
    Config config;

    for (const auto& [path, generated] : _generated_from) {
      config.write_to_section(path);
      config.write("generated_files", string_vector_combine(generated, ";"));
    }

    return config.write_to_file(p_path);
  }

  void ExecutionContext::clear_generated_from() { _generated_from.clear(); }

  void ExecutionContext::clear_last_modified_times() { _last_modified_times.clear(); }

  void ExecutionContext::clean_orphan_generated_files() {
    auto itr = _generated_from.begin();
    while (itr != _generated_from.end()) {
      const auto& [path, generated_files] = *itr;

      if (!file_exists(path)) {
        for (const String& generated_file : generated_files) {
          if (file_exists(generated_file)) {
            remove_file(generated_file);
          }
        }
        itr = _generated_from.erase(itr);
        continue;
      }
      itr++;
    }
  }

  bool ExecutionContext::clear_generated_files(const String& p_path) {
    auto itr = _generated_from.find(p_path);

    if (itr == _generated_from.end()) {
      return false;
    }

    for (const String& generated : itr->second) {
      if (file_exists(generated)) {
        remove_file(generated);
      }
    }
    _generated_from.erase(p_path);
    return true;
  }

  bool ExecutionContext::load_last_modified_times_file(const String& p_path) {
    Config config;
    if (!config.read_from_file(p_path)) {
      return false;
    }

    _last_modified_times.clear();
    _out_last_modified_times.clear();

    for (const String& section : config.get_sections()) {
      config.read_from_section(section);
      if (config.has_config_value("last_modified")) {
        _last_modified_times[section] = config.read<String, Size>("last_modified");
        _out_last_modified_times[section] = config.read<String, Size>("last_modified");
      }
    }

    return true;
  }

  bool ExecutionContext::save_last_modified_times_file(const String& p_path) {
    Config config;

    for (const auto& [path, last_modified] : _out_last_modified_times) {
      config.write_to_section(path);
      config.write<String, Size>("last_modified", last_modified);
    }

    return config.write_to_file(p_path);
  }

  bool ExecutionContext::file_modified(const String& p_path, bool p_update_time) {
    String absolute = path_absolute(p_path);

    Size last_modified = file_write_time(absolute);

    if (last_modified == 0) {
      return false;
    }

    bool modified = false;

    auto itr = _last_modified_times.find(absolute);
    if (itr == _last_modified_times.end()) {
      modified = true;
    } else {
      modified = (*itr).second != last_modified;
    }

    if (p_update_time) {
      _out_last_modified_times[absolute] = last_modified;
    }
    return modified;
  }

  void ExecutionContext::set_usings(const Vector<String>& p_value) { _usings = p_value; }

  const Vector<String>& ExecutionContext::get_usings() { return _usings; }

  void ExecutionContext::add_using(const String& p_value) { _usings.push_back(p_value); }

  void ExecutionContext::remove_using(const String& p_value) {
    auto itr = std::find(_usings.begin(), _usings.end(), p_value);
    if (itr != _usings.end()) {
      _usings.erase(itr);
    }
  }

  void ExecutionContext::clear_usings() { _usings.clear(); }

}

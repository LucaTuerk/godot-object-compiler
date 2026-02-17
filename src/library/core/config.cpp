/**************************************************************************/
/* config.cpp                                                             */
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

#include "library/core/config.h"

namespace GodotObjectCompiler {

  Config::Config() { write_to_section<String>(""); }

  const Vector<String>& Config::get_sections() { return _sections; }

  bool Config::has_config_value(const String& p_key) {
    return config_values[_current_section].find(p_key) != config_values[_current_section].end();
  }

  void Config::_write_to_section(const String& p_section) {
    _current_section = _find_section_index(p_section);
    if (_current_section == INVALID_ID) {
      _current_section = _sections.size();
      _section_indices[p_section] = _current_section;
      _sections.push_back(p_section);
    }
  }

  void Config::_write(const String& p_key, const String& p_value) { config_values[_current_section][p_key] = p_value; }

  void Config::_read_from_section(const String& p_section) {
    _current_section = _find_section_index(p_section);
    if (_current_section == INVALID_ID) {
      _current_section = 0;
    }
  }

  String Config::_read(const String& p_key) { return config_values[_current_section][p_key]; }

  Size Config::_find_section_index(const String& p_key) {
    auto itr = _section_indices.find(p_key);
    if (itr != _section_indices.end()) {
      return itr->second;
    }
    return INVALID_ID;
  }

  bool Config::write_to_file(const String& p_path) {
    std::ofstream file(p_path);
    if (!file.is_open()) {
      return false;
    }

    for (const auto& section : config_values) {
      file << "[" << _sections[section.first] << "]" << std::endl;
      for (const auto& value : section.second) {
        file << value.first << "=" << value.second << std::endl;
      }
      file << std::endl;
    }
    return true;
  }

  bool Config::read_from_file(const String& p_path) {
    std::ifstream file(p_path);
    if (!file.is_open()) {
      return false;
    }

    String line;

    while (std::getline(file, line)) {
      if (line.empty() || line[0] == '#') {
        continue;
      } else if (line[0] == '[') {
        String section = line.substr(1, line.find(']') - 1);
        write_to_section(section);
      } else {
        size_t pos = line.find('=');
        if (pos != String::npos) {
          String key = line.substr(0, pos);
          String value = line.substr(pos + 1);
          write(key, value);
        }
      }
    }
    return true;
  }

}

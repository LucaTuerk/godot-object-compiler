#include "config.h"

#include <fstream>

#include "library/core/config.h"

namespace GodotObjectCompiler {

  Config::Config() { write_to_section<String>(""); }

  const Vector<String>& Config::get_sections() { return _sections; }

  bool Config::has_config_value(const String& key) {
    return config_values[_current_section].find(key) != config_values[_current_section].end();
  }

  void Config::_write_to_section(const String& section) {
    _current_section = _find_section_index(section);
    if (_current_section == INVALID_ID) {
      _current_section = _sections.size();
      _section_indices[section] = _current_section;
      _sections.push_back(section);
    }
  }

  void Config::_write(const String& key, const String& value) { config_values[_current_section][key] = value; }

  void Config::_read_from_section(const String& section) {
    _current_section = _find_section_index(section);
    if (_current_section == INVALID_ID) {
      _current_section = 0;
    }
  }

  String Config::_read(const String& key) { return config_values[_current_section][key]; }

  Size Config::_find_section_index(const String& key) {
    auto itr = _section_indices.find(key);
    if (itr != _section_indices.end()) {
      return itr->second;
    }
    return INVALID_ID;
  }

  bool Config::write_to_file(const String& path) {
    std::ofstream file(path);
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

  bool Config::read_from_file(const String& path) {
    std::ifstream file(path);
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

}  // namespace GodotObjectCompiler

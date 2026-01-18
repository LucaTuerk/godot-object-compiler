#include "library/core/config.h"
#include <fstream>

namespace GodotObjectCompiler {
    Config::Config() {
        switch_section<String>("");
    }

    bool Config::has_config_value(const String& key) {
        return config_values[current_section].find(key) != config_values[current_section].end();
    }

    void Config::write_to_file(const String& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        for (const auto& section : config_values) {
            file << "[" << section.first << "]" << std::endl;
            for (const auto& value : section.second) {
                file << value.first << "=" << value.second << std::endl;
            }
            file << std::endl;
        }
    }

    Config Config::read_from_file(const String& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for reading");
        }

        Config config;
        String line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            } else if (line[0] == '[') {
                String section = line.substr(1, line.find(']') - 1);
                config.switch_section(section);
            } else {
                size_t pos = line.find('=');
                if (pos != String::npos) {
                    String key = line.substr(0, pos);
                    String value = line.substr(pos + 1);
                    config.set(key, value);
                }
            }
        }

        return config;
    }
}

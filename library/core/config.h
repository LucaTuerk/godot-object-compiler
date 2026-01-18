#pragma once

#include "library/core/core.h"
#include <sstream>
#include <string>

namespace GodotObjectCompiler {
    class Config {
        public:
            Config();
            ~Config() = default;

            bool has_config_value(const String& key);

            template<typename T>
            void switch_section(const T& section);

            template<typename T>
            T get(const String& key);

            template<typename T>
            T get(const String& key, const T& default_value);

            template<typename T>
            void set(const String& key, const T& value);

            void write_to_file(const String& path);
            static Config read_from_file(const String& path);

        private:
            String current_section;
            Dictionary<String, Dictionary<String, String>> config_values;
    };

}


template<typename T>
void GodotObjectCompiler::Config::switch_section(const T& section) {
    std::stringstream strstr;
    strstr << section;
    current_section = strstr.str();

    if(config_values.find(current_section) == config_values.end()) {
        config_values[current_section] = Dictionary<String, String>();
    }
}

template<typename T>
T GodotObjectCompiler::Config::get(const String& key) {
    String value = config_values[current_section][key];

    std::istringstream ss(value);
    T result;
    ss >> result;
    return result;
}

template<typename T>
T GodotObjectCompiler::Config::get(const String& key, const T& default_value) {
    if (!has_config_value(key)) {
        return default_value;
    }
    return get<T>(key);
}

template<typename T>
void GodotObjectCompiler::Config::set(const String& key, const T& value) {
    std::stringstream strstr;
    strstr << value;
    config_values[current_section][key] = strstr.str();
}

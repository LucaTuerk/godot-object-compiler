
#include "library/core/core.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <cerrno>
#include <iostream>

String read_file(const String& path) {
    std::ifstream ifs;
    //prepare f to throw if failbit gets set
    std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
    ifs.exceptions(exceptionMask);

    try {
      ifs.open(path);
    }
    catch (std::ios_base::failure& e) {
      std::cerr << e.what() << '\n';
      return "";
    }

    std::string str(std::istreambuf_iterator<char>{ifs}, {});
    return str;
}

void write_file(const String& path, const String& content) {
    std::ofstream ofs(path.c_str(), std::ios::out | std::ios::binary);
    ofs.write(content.c_str(), content.size());
}

String generate_random_string(size_t length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}
bool string_contains(const String &str, const String &str2) {
	return str.find(str2) != String::npos;
}

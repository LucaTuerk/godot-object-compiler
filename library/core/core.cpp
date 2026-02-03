
#include "library/core/core.h"

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <random>

#include "permissions.h"

String read_file(const String& path) {
  std::ifstream ifs;
  // prepare f to throw if failbit gets set
  std::ios_base::iostate exceptionMask = ifs.exceptions() | std::ios::failbit;
  ifs.exceptions(exceptionMask);

  try {
    ifs.open(path);
  } catch (std::ios_base::failure& e) {
    std::cerr << e.what() << '\n';
    return "";
  }

  std::string str(std::istreambuf_iterator<char>{ifs}, {});
  return str;
}

Vector<String> read_lines(const String& path) {
  Vector<String> result;
  std::ifstream ifs{path};

  for (std::string line; std::getline(ifs, line);) {
    result.emplace_back(line);
  }

  return result;
}

void write_file(const String& path, const String& content) {
  GodotObjectCompiler::Permissions::instance()->ensure_is_allowed_write_path(path);
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

bool string_contains(const String& str, const String& str2) { return str.find(str2) != String::npos; }

bool string_suffix(const String& str, const String& suffix) { return str.rfind(suffix) == str.size() - suffix.size(); }

bool string_only_contains(const String& str, char symbol) {
  if (str.length() == 0) {
    return false;
  }

  for (char c : str) {
    if (c != symbol) {
      return false;
    }
  }

  return true;
}

Vector<String> string_split(const String& str, const String& delimiter) {
  Vector<String> result;

  Size start = 0;
  Size end = 0;
  Size length = str.length();

  do {
    end = str.find(delimiter, start);
    result.emplace_back(str.substr(start, end - start));
    start = end + delimiter.length();
  } while (end < length);

  if (result.empty()) {
    result.emplace_back(str);
  }

  return result;
}

void print_columns(const std::array<Size, 3>& column_size, std::vector<String>&& content) {}
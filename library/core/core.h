#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>


using String = std::string;

template<typename T>
using Vector = std::vector<T>;

using Size = std::size_t;
using UID = std::size_t;
static const UID INVALID_ID = 0;

template<typename K, typename V>
using Dictionary = std::map<K, V>;

using TypeInfo = std::type_info;
using TypeIndex = std::type_index;

String read_file(const String& path);
void write_file(const String& path, const String& content);
String generate_random_string(size_t length);

bool string_contains(const String& str, const String& str2);
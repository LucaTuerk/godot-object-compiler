#pragma once

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

using Size = std::size_t;
using UID = std::size_t;
using Index = std::size_t;
using SignedIndex = int32_t;

static const UID INVALID_ID = 0;
static const Index INVALID_INDEX = -1;

template <typename K, typename V>
using Dictionary = std::map<K, V>;

using TypeInfo = std::type_info;
using TypeIndex = std::type_index;

String read_file(const String& path);
void write_file(const String& path, const String& content);
String generate_random_string(size_t length);

bool string_contains(const String& str, const String& str2);
bool string_suffix(const String& str, const String& str2);
bool string_only_contains(const String& str, char symbol);

void print_ln(const String& str);
void print_err(const String& str);
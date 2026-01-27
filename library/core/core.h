#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// This is the ugly file

using String = std::string;

template <typename T>
using Limits = std::numeric_limits<T>;

template <typename T>
using Predicate = std::function<bool(T*)>;

template <typename T>
using Creator = std::function<T*()>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using List = std::list<T>;

template <typename T>
using HashSet = std::set<T>;

template <typename T>
using Hasher = std::hash<T>;

using TimePoint = std::chrono::steady_clock::time_point;

using Hash = std::size_t;
using Size = std::size_t;
using UID = std::size_t;
using Index = std::size_t;
using SignedIndex = int32_t;

static const UID INVALID_ID = 0;
static const Index INVALID_INDEX = -1;

template <typename K, typename V>
using Dictionary = std::map<K, V>;

template <typename A, typename B>
using Pair = std::pair<A, B>;

using TypeInfo = std::type_info;
using TypeIndex = std::type_index;

template <typename T>
void print(const T& message) {
  std::cout << message;
}

template <typename T>
void print_ln(const T& message) {
  std::cout << message << std::endl;
}

template <typename T>
void print_err(const T& message) {
  std::cerr << message << std::endl;
}

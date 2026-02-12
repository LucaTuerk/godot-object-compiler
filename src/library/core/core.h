/**************************************************************************/
/* core.h                                                                 */
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
#pragma once

#include "library/core/system_includes.h"

namespace GodotObjectCompiler {

  using String = std::string;

  template <typename T>
  using Limits = std::numeric_limits<T>;

  template <typename T>
  using Vector = std::vector<T>;

  template <typename T>
  using List = std::list<T>;

  template <typename T>
  using HashSet = std::set<T>;

  template <typename T>
  using Hasher = std::hash<T>;

  template <typename T>
  using Ref = std::shared_ptr<T>;

  template <typename T>
  using WeakRef = std::weak_ptr<T>;

  template <typename T, typename... Args>
  Ref<T> make_ref(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using Predicate = std::function<bool(Ref<T>)>;

  template <typename T>
  using Creator = std::function<Ref<T>()>;

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

  String format(const String& p_format_string);

  template <typename... Args>
  String format(const String& format_str, Args&&... args);

  template <typename... Args>
  void fmt_print(const String& format_str, Args&&... args);

  template <typename... Args>
  void fmt_print_ln(const String& format_str, Args&&... args);

  template <typename... Args>
  void fmt_print_err(const String& format_str, Args&&... args);

}

template <typename... Args>
GodotObjectCompiler::String GodotObjectCompiler::format(const String& format_str, Args&&... args) {
  const std::size_t n = sizeof...(Args);
  if (n == 0) {
    return format_str;
  }

  int size_s = std::snprintf(nullptr, 0, format_str.c_str(), args...) + 1;
  if (size_s <= 0) {
    return "";
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format_str.c_str(), args...);
  return String(buf.get(), buf.get() + size - 1);
}

template <typename... Args>
void GodotObjectCompiler::fmt_print(const String& format_str, Args&&... args) {
  print(format(format_str, std::forward<Args>(args)...));
}

template <typename... Args>
void GodotObjectCompiler::fmt_print_err(const String& format_str, Args&&... args) {
  print_err(format(format_str, std::forward<Args>(args)...));
}

template <typename... Args>
void GodotObjectCompiler::fmt_print_ln(const String& format_str, Args&&... args) {
  print_ln(format(format_str, std::forward<Args>(args)...));
}

#define UNUSED(param) (void)param

#define PANIC(...)                                                               \
  print_err(format("PANIC!! %s:%d ", __FILE__, __LINE__) + format(__VA_ARGS__)); \
  abort()

#define PANIC_COND(condition, ...) \
  if ((condition)) {               \
    PANIC(__VA_ARGS__);            \
  }

#ifdef PANIC_ON_ERR
#define ERR(...) PANIC(__VA_ARGS__)
#else
#define ERR(...) print_err(format("%s:%d ", __FILE__, __LINE__) + format(__VA_ARGS__));
#endif
#define ERR_COND(condition, ...) \
  if ((condition)) {             \
    ERR(__VA_ARGS__);            \
  }

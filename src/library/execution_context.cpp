/**************************************************************************/
/* execution_context.cpp                                                  */
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

#include "execution_context.h"

#include "core/file_system_utilities.h"
#include "core/string_utilities.h"
#include "parser/parser.h"
#include "tree/syntax/namespace.h"
#include "type_db.h"

namespace GodotObjectCompiler {

  ExecutionContext::ExecutionContext() { init(); }

  String error_level_to_string(ErrorLevel level) {
    switch (level) {
      case ERROR:
        return "Error";
        break;
      case WARNING:
        return "Warning";
        break;
      case INFO:
        return "Info";
        break;
      case VERBOSE:
        return "Verbose";
        break;
    }
    return "";
  }

  void ExecutionContext::init() {
    _node_db = {};
    _include_paths = {};
    _remove_macros = {};
  }

  NodeDB* ExecutionContext::get_node_db() { return &_node_db; }

  const Vector<String>& ExecutionContext::get_remove_macros() { return _remove_macros; }

  void ExecutionContext::set_remove_macros(const Vector<String>& p_value) { _remove_macros = p_value; }

  const Vector<String>& ExecutionContext::get_include_paths() { return _include_paths; }

  void ExecutionContext::set_include_paths(const Vector<String>& p_value) { _include_paths = p_value; }

  void ExecutionContext::set_error_level(ErrorLevel p_level, ErrorDetail p_error_detail) {
    _error_level = p_level;
    _error_detail = p_error_detail;
  }

  ErrorLevel ExecutionContext::get_error_level() const { return _error_level; }

  ErrorDetail ExecutionContext::get_error_detail() const { return _error_detail; }

  void ExecutionContext::print(ErrorLevel p_level, const String& p_message) const {
    if (p_level >= _error_level) {
      print_ln(p_message);
    }
  }

  bool ExecutionContext::is_cached(const String& p_path) {
    // TODO: generalize
    return file_exists(p_path);
  }

  Hash ExecutionContext::get_path_hash(const String& p_absolute_path) {
    Hasher<String> hasher;
    return hasher(p_absolute_path);
  }

  String ExecutionContext::get_cache_file_path(Hash p_hash) {
    // TODO: generalize
    return path_concat(".goc/cache", hash_string(p_hash) + ".gocdb");
  }

}
//
// Created by luca on 20.01.26.
//

#include "execution_context.h"

#include <filesystem>

#include "core/helpers.h"
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
    _included_files = {};
    _include_paths = {};
    _remove_macros = {};
  }

  NodeDB* ExecutionContext::get_node_db() { return &_node_db; }

  const Vector<String>& ExecutionContext::get_remove_macros() { return _remove_macros; }

  void ExecutionContext::set_remove_macros(const Vector<String>& p_value) { _remove_macros = p_value; }

  String ExecutionContext::get_absolute_include_path(const String& p_included_from_path, const String& p_path) const {
    const String base_path = path_base(p_included_from_path);

    if (String relative_absolute = path_concat(base_path, p_path); file_exists(relative_absolute)) {
      return relative_absolute;
    }

    for (const String& include_path : _include_paths) {
      if (String absolute = path_concat(include_path, p_path); file_exists(absolute)) {
        return absolute;
      }
    }
    return "";
  }

  const Vector<String>& ExecutionContext::get_include_paths() { return _include_paths; }

  Ref<Node> ExecutionContext::get_include(const String& p_included_from_path, const String& p_path) {
    const String absolute = get_absolute_include_path(p_included_from_path, p_path);

    if (absolute.empty()) {
      print_err("Could not find absolute path for include \"" + p_path + "\" included from file \"" +
                p_included_from_path + "\"");
    }

    if (is_file_included(absolute)) {
      return nullptr;
    }
    set_file_included(absolute);

    Ref<Node> root = nullptr;
    const Hash absolute_hash = get_path_hash(absolute);
    if (const auto incl = _included_nodes.find(absolute_hash); incl != _included_nodes.end()) {
      return incl->second->clone();
    }

    String cache_path = get_cache_file_path(absolute_hash);

    ConfigNodeReaderWriter reader_writer;
    if (file_exists(cache_path)) {
      root = reader_writer.read_from_file(cache_path);
    } else {
      TreeSitterParser parser;
      const Ref<Namespace> global_namespace = node_new<Namespace>();

      if (Ref<ParserError> error = parser.parse_file(p_path, global_namespace); error != ParserError::OK) {
        return nullptr;
      }

      root = global_namespace;
      reader_writer.write_to_file(root, cache_path);
    }

    _included_nodes[absolute_hash] = root;
    return root->clone();
  }

  void ExecutionContext::set_include_paths(const Vector<String>& p_value) { _include_paths = p_value; }

  bool ExecutionContext::is_file_included(const String& p_include_path) {
    return _included_files.find(p_include_path) != _included_files.end();
  }

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

  void ExecutionContext::set_file_included(const String& p_include_path) { _included_files.insert(p_include_path); }

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

}  // namespace GodotObjectCompiler
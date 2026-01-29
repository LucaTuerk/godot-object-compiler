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

  void ExecutionContext::set_remove_macros(const Vector<String>& value) { _remove_macros = value; }

  String ExecutionContext::get_absolute_include_path(const String& included_from_path, const String& path) const {
    String base_path = path_base(included_from_path);
    String relative_absolute = path_concat(base_path, path);

    if (file_exists(relative_absolute)) {
      return relative_absolute;
    }

    for (const String& include_path : _include_paths) {
      String absolute = path_concat(include_path, path);
      if (file_exists(absolute)) {
        return absolute;
      }
    }
    return "";
  }

  const Vector<String>& ExecutionContext::get_include_paths() { return _include_paths; }

  Ref<Node> ExecutionContext::get_include(const String& included_from_path, const String& path) {
    String absolute = get_absolute_include_path(included_from_path, path);

    if (absolute.empty()) {
      print_err("Could not find absolute path for include \"" + path + "\" included from file \"" + included_from_path +
                "\"");
    }

    if (is_file_included(absolute)) {
      return nullptr;
    }
    set_file_included(absolute);

    Ref<Node> root = nullptr;
    Hash absolute_hash = get_path_hash(absolute);
    if (auto incl = _included_nodes.find(absolute_hash); incl != _included_nodes.end()) {
      return incl->second->clone();
    }

    String cache_path = get_cache_file_path(absolute_hash);

    ConfigNodeReaderWriter reader_writer;
    if (file_exists(cache_path)) {
      root = reader_writer.read_from_file(cache_path);
    } else {
      TreeSitterParser parser;
      Ref<Namespace> global_namespace = node_new<Namespace>();

      Ref<ParserError> error = parser.parse_file(path, global_namespace);
      if (error != ParserError::OK) {
        return nullptr;
      }

      root = global_namespace;
      reader_writer.write_to_file(root, cache_path);
    }

    _included_nodes[absolute_hash] = root;
    return root->clone();
  }

  void ExecutionContext::set_include_paths(const Vector<String>& value) { _include_paths = value; }

  bool ExecutionContext::is_file_included(const String& include_path) {
    return _included_files.find(include_path) != _included_files.end();
  }

  void ExecutionContext::set_error_level(ErrorLevel level, ErrorDetail error_detail) {
    _error_level = level;
    _error_detail = error_detail;
  }

  ErrorLevel ExecutionContext::get_error_level() const { return _error_level; }

  ErrorDetail ExecutionContext::get_error_detail() const { return _error_detail; }

  void ExecutionContext::print(ErrorLevel level, const String& message) {
    if (level >= _error_level) {
      print_ln(message);
    }
  }

  void ExecutionContext::set_file_included(const String& include_path) { _included_files.insert(include_path); }

  bool ExecutionContext::is_cached(const String& path) {
    // TODO: generalize
    return file_exists(path);
  }

  Hash ExecutionContext::get_path_hash(const String& absolute_path) {
    Hasher<String> hasher;
    return hasher(absolute_path);
  }

  String ExecutionContext::get_cache_file_path(Hash hash) {
    // TODO: generalize
    return path_concat(".goc/cache", hash_string(hash) + ".gocdb");
  }

}  // namespace GodotObjectCompiler
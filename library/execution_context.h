#pragma once
#include "core/core.h"
#include "node_db.h"

namespace GodotObjectCompiler {

  enum ErrorLevel {
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    VERBOSE = 3,
  };

  enum ErrorDetail {
    CONDENSED = 0,
    FULL = 1,
  };

  String error_level_to_string(ErrorLevel level);

  class ExecutionContext {
   public:

    static ExecutionContext* instance() {
      static ExecutionContext singleton = ExecutionContext();
      return &singleton;
    }

    void init();

    NodeDB* get_node_db();

    const Vector<String>& get_remove_macros();

    void set_remove_macros(const Vector<String>& value);

    String get_absolute_include_path(const String& included_from_path, const String& path) const;

    const Vector<String>& get_include_paths();

    Ref<Node> get_include(const String& included_from_path, const String& path);

    void set_include_paths(const Vector<String>& value);

    bool is_file_included(const String& include_path);

    void set_error_level(ErrorLevel level, ErrorDetail error_detail);

    ErrorLevel get_error_level() const;

    ErrorDetail get_error_detail() const;

    void print(ErrorLevel level, const String& message);

   private:

    void set_file_included(const String& include_path);

    bool is_cached(const String& path);

    static Hash get_path_hash(const String& absolute_path);

    String get_cache_file_path(Hash hash);

    ExecutionContext();

    Dictionary<Hash, Ref<Node>> _included_nodes;

    /** NODES **/
    NodeDB _node_db;

    /** FILES **/
    Vector<String> _input_files;

    /** MACROS **/
    Vector<String> _remove_macros;

    /** INCLUDE **/
    Vector<String> _include_paths;
    HashSet<String> _included_files;

    ErrorLevel _error_level;
    ErrorDetail _error_detail;
  };

}  // namespace GodotObjectCompiler

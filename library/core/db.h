#pragma once
#include "core.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  class Node;
  class Namespace;

  class DB {
   public:

    static DB init(Namespace* root = nullptr);
    static DB read_from_config(const String& path);

    void write_to_config(const String& path) const;
    Namespace* get_root() const;

   private:

    Namespace* _root = nullptr;
  };

  class TypeDB {
   public:

    static TypeDB* instance();
    void set_cache_directory(const String& path);
    void save_type_data(Namespace* root);
    Node* get_type_data(const String& qualified_name);

   private:

    [[nodiscard]] String _get_cache_file_path(const String& qualified_name) const;
    Dictionary<String, Node*> _cache;
    String _cache_directory;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "core.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  class Node;
  class Namespace;

  class DB {
   public:

    static DB init(Ref<Namespace> root = nullptr);
    static DB read_from_config(const String& path);

    void write_to_config(const String& path) const;
    Ref<Namespace> get_root() const;

   private:

    Ref<Namespace> _root = nullptr;
  };

  class TypeDB {
   public:

    static TypeDB* instance();
    void set_cache_directory(const String& path);
    void save_type_data(Ref<Namespace> root);
    Ref<Node> get_type_data(const String& qualified_name);

   private:

    [[nodiscard]] String _get_cache_file_path(const String& qualified_name) const;
    Dictionary<String, Ref<Node>> _cache;
    String _cache_directory;
  };

}  // namespace GodotObjectCompiler

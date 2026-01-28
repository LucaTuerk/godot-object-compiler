#pragma once
#include "core/core.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  class ConfigNodeReaderWriter : public INodeReader, public INodeWriter {
   public:

    bool write_to_file(Ref<Node> node, const String& path) override;
    Ref<Node> read_from_file(const String& path) override;
  };

  class TypeDB {
   public:

    static TypeDB* instance();
    void set_cache_directory(const String& path);
    void save_type_data(Ref<NamedContext> root);
    Ref<Node> get_type_data(const String& qualified_name);

    template <typename T>
    Ref<T> get_type_data(const String& qualified_name);

   private:

    using Reader = ConfigNodeReaderWriter;
    using Writer = ConfigNodeReaderWriter;

    [[nodiscard]] String _get_cache_file_path(const String& qualified_name) const;
    Dictionary<String, Ref<Node>> _cache;
    String _cache_directory;
  };

  template <typename T>
  Ref<T> TypeDB::get_type_data(const String& qualified_name) {
    Ref<Node> result = get_type_data(qualified_name);
    if (!result) {
      return nullptr;
    }

    return result->as<T>();
  }

}  // namespace GodotObjectCompiler

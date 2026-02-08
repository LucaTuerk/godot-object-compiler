
#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  class Resources {
   public:

    using ResourcePack = Dictionary<String, const char*>;

    static Resources* instance() {
      static Resources instance;
      return &instance;
    }

    void load_pack(ResourcePack* p_pack);

    Vector<String> resources_recursive(const String& p_path) const;

    [[nodiscard]] String load_text_resource(const String& p_path) const;

    [[nodiscard]] bool has_resource(const String& p_path) const;

   private:

    Vector<ResourcePack*> _loaded_packs;
  };

}  // namespace GodotObjectCompiler

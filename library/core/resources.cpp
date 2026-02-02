
#include "resources.h"

#include "helpers.h"

namespace GodotObjectCompiler {

  void Resources::load_pack(ResourcePack* pack) { _loaded_packs.push_back(pack); }

  Vector<String> Resources::resources_recursive(const String& path) const {
    Vector<String> res;
    for (const ResourcePack* pack : _loaded_packs) {
      for (const auto& [res_path, _] : *pack) {
        if (string_prefix(res_path, path)) {
          res.push_back(res_path);
        }
      }
    }
    return res;
  }

  String Resources::load_text_resource(const String& path) const {
    for (ResourcePack* pack : _loaded_packs) {
      if (auto itr = pack->find(path); itr != pack->end()) {
        return String(itr->second);
      }
    }
    return "";
  }

  bool Resources::has_resource(const String& path) const {
    for (ResourcePack* pack : _loaded_packs) {
      if (auto itr = pack->find(path); itr != pack->end()) {
        return true;
      }
    }
    return false;
  }

}  // namespace GodotObjectCompiler
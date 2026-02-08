
#include "resources.h"

#include "helpers.h"

namespace GodotObjectCompiler {

  void Resources::load_pack(ResourcePack* p_pack) { _loaded_packs.push_back(p_pack); }

  Vector<String> Resources::resources_recursive(const String& p_path) const {
    Vector<String> res;
    for (const ResourcePack* pack : _loaded_packs) {
      for (const auto& [res_path, _] : *pack) {
        if (string_prefix(res_path, p_path)) {
          res.push_back(res_path);
        }
      }
    }
    return res;
  }

  String Resources::load_text_resource(const String& p_path) const {
    for (ResourcePack* pack : _loaded_packs) {
      if (auto itr = pack->find(p_path); itr != pack->end()) {
        return String(itr->second);
      }
    }
    return "";
  }

  bool Resources::has_resource(const String& p_path) const {
    for (ResourcePack* pack : _loaded_packs) {
      if (auto itr = pack->find(p_path); itr != pack->end()) {
        return true;
      }
    }
    return false;
  }

}  // namespace GodotObjectCompiler
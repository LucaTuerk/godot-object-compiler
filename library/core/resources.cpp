
#include "resources.h"

namespace GodotObjectCompiler {

  void Resources::load_pack(ResourcePack* pack) { _loaded_packs.push_back(pack); }

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
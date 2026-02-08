
#include "predicates.h"

namespace GodotObjectCompiler {

  Predicate<TreeSitterNode> TreeSitterNodePredicates::types(std::initializer_list<const char*>&& types) {
    return [types](Ref<TreeSitterNode> node) {
      for (const char* name : types) {
        if (name == node->type) {
          return true;
        }
      }
      return false;
    };
  }

  Predicate<TreeSitterNode> TreeSitterNodePredicates::type(const char* type) {
    return [type](Ref<TreeSitterNode> node) { return node->type == type; };
  };

}  // namespace GodotObjectCompiler

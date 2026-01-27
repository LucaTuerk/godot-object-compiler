//
// Created by luca on 20.01.26.
//

#include "attribute.h"

namespace GodotObjectCompiler {

  Node* Attribute::resolve_target() const {
    switch (_get_target()) {
      case NEXT: {
        Node* target = get_next_sibling();
        if (verify_target(target)) {
          return target;
        }
        return nullptr;
      } break;
      case CONTAINING: {
        return find_parent<Node>([this](Node* p_parent) { return verify_target(p_parent); });
      } break;
      default:
        print_err("UNIMPLEMENTED");
        return nullptr;
        break;
    }
  }

  bool Attribute::verify_target(Node* p_resolved) const {
    if (!_verify_target_class(p_resolved)) {
      print_err("WRONG CLASS LUL");
      return false;
    }

    return _verify_target(p_resolved);
  }

  bool Attribute::_verify_target(Node* p_resolved) const { return true; }

}  // namespace GodotObjectCompiler
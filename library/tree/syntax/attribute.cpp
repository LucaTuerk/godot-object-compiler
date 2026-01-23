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

  bool AttributeDB::register_attribute(const String& class_name, const String& macro, CreationFunc creator) {
    _macro_aliases[macro] = class_name;
    _creation_funcs[class_name] = creator;
    return true;
  }

  bool AttributeDB::register_attribute_params(
      const String& class_name, const String& override_name, std::initializer_list<AttributeParameterType>&& args) {
    _parameter_type[class_name][override_name] = std::move(args);
    return true;
  }

  bool AttributeDB::is_known_macro(const String& macro) { return _macro_aliases.find(macro) != _macro_aliases.end(); }

  Attribute* AttributeDB::create_for_macro(const String& macro) {
    auto name_itr = _macro_aliases.find(macro);
    if (name_itr == _macro_aliases.end()) {
      return nullptr;
    }

    auto creator_itr = _creation_funcs.find(name_itr->second);
    if (creator_itr == _creation_funcs.end()) {
      return nullptr;
    }

    return creator_itr->second();
  }

  Vector<String> AttributeDB::get_all_macros() {
    Vector<String> macros;
    for (auto& [key, val] : _macro_aliases) {
      macros.push_back(key);
    }
    return macros;
  }

  Dictionary<String, Vector<AttributeParameterType>> const* AttributeDB::get_parameter_types(const String& macro) {
    static Dictionary<String, Vector<AttributeParameterType>> empty_res = {};

    auto name_itr = _macro_aliases.find(macro);
    if (name_itr == _macro_aliases.end()) {
      return &empty_res;
    }

    auto parameter_itr = _parameter_type.find(name_itr->second);
    if (parameter_itr == _parameter_type.end()) {
      return &empty_res;
    }

    return &parameter_itr->second;
  }

}  // namespace GodotObjectCompiler
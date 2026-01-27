
#include "attribute_db.h"

#include "library/generator/generator.h"

namespace GodotObjectCompiler {

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

  bool AttributeDB::register_parameter(const String& class_name, IAttributeParameters* parameter) {
    const HashSet<String>& registered = _registered_parameters[class_name];
    if (registered.find(parameter->return_type_name()) != registered.end()) {
      return false;
    }

    _parameters[class_name].push_back(parameter);
    return true;
  }

  Vector<IAttributeParameters*> AttributeDB::get_parameters(const String& macro) {
    if (auto alias_itr = _macro_aliases.find(macro); alias_itr != _macro_aliases.end()) {
      if (auto itr = _parameters.find(alias_itr->second); itr != _parameters.end()) {
        return itr->second;
      }
    }
    return {};
  }

  bool AttributeDB::register_class_generator(const String& generator_name, ClassGenerator* generator) {
    if (_registered_generator_names.find(generator_name) == _registered_generator_names.end()) {
      _registered_generator_names.insert(generator_name);
      _class_generators.push_back(generator);
      return true;
    }
    return false;
  }

  const Vector<ClassGenerator*>& AttributeDB::class_generators() const { return _class_generators; }

}  // namespace GodotObjectCompiler
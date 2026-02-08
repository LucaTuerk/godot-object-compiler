
#include "attribute_db.h"

#include "library/generator/generator.h"

namespace GodotObjectCompiler {

  bool AttributeDB::register_attribute(const String& p_class_name, const String& p_macro, CreationFunc p_creator) {
    _macro_aliases[p_macro] = p_class_name;
    _creation_funcs[p_class_name] = p_creator;
    return true;
  }

  bool AttributeDB::register_attribute_parameter(
      const String& p_class_name, const Ref<IAttributeParameterType>& p_parameter) {
    HashSet<String>& registered = _registered_parameter_types[p_class_name];
    if (registered.find(p_parameter->get_type()) != registered.end()) {
      return false;
    }

    registered.insert(p_parameter->get_type());
    _parameters[p_class_name].push_back(p_parameter);
    return true;
  }

  bool AttributeDB::is_known_macro(const String& p_macro) {
    return _macro_aliases.find(p_macro) != _macro_aliases.end();
  }

  Ref<Attribute> AttributeDB::create_for_macro(const String& p_macro) {
    const auto name_itr = _macro_aliases.find(p_macro);
    if (name_itr == _macro_aliases.end()) {
      return nullptr;
    }

    const auto creator_itr = _creation_funcs.find(name_itr->second);
    if (creator_itr == _creation_funcs.end()) {
      return nullptr;
    }

    return creator_itr->second();
  }

  Vector<Ref<IAttributeParameterType>> AttributeDB::get_parameters_for_macro(const String& p_macro) {
    const auto name_itr = _macro_aliases.find(p_macro);
    if (name_itr == _macro_aliases.end()) {
      return {};
    }

    return _parameters[name_itr->second];
  }

  String AttributeDB::get_macro_for_attribute(const String& p_class_name) {
    for (const auto& [macro, name] : _macro_aliases) {
      if (name == p_class_name) {
        return macro;
      }
    }
    return "";
  }

  Vector<String> AttributeDB::get_all_macros() {
    Vector<String> macros;
    for (auto& [key, val] : _macro_aliases) {
      macros.push_back(key);
    }
    return macros;
  }

  bool AttributeDB::register_class_generator(const String& p_generator_name, Ref<ClassGenerator> p_generator) {
    if (_registered_generator_names.find(p_generator_name) == _registered_generator_names.end()) {
      _registered_generator_names.insert(p_generator_name);
      _class_generators.push_back(p_generator);
      return true;
    }
    return false;
  }

  const Vector<Ref<ClassGenerator>>& AttributeDB::class_generators() const { return _class_generators; }

}  // namespace GodotObjectCompiler

#include "attribute_db.h"

#include "library/generator/generator.h"

namespace GodotObjectCompiler {

  bool AttributeDB::register_attribute(const String& class_name, const String& macro, CreationFunc creator) {
    _macro_aliases[macro] = class_name;
    _creation_funcs[class_name] = creator;
    return true;
  }

  bool AttributeDB::register_attribute_parameter(
      const String& class_name, const Ref<IAttributeParameterType>& parameter) {
    HashSet<String>& registered = _registered_parameter_types[class_name];
    if (registered.find(parameter->get_type_name()) != registered.end()) {
      return false;
    }

    registered.insert(parameter->get_type_name());
    _parameters[class_name].push_back(parameter);
    return true;
  }

  bool AttributeDB::is_known_macro(const String& macro) { return _macro_aliases.find(macro) != _macro_aliases.end(); }

  Ref<Attribute> AttributeDB::create_for_macro(const String& macro) {
    const auto name_itr = _macro_aliases.find(macro);
    if (name_itr == _macro_aliases.end()) {
      return nullptr;
    }

    const auto creator_itr = _creation_funcs.find(name_itr->second);
    if (creator_itr == _creation_funcs.end()) {
      return nullptr;
    }

    return creator_itr->second();
  }

  Vector<Ref<IAttributeParameterType>> AttributeDB::get_parameters_for_macro(const String& macro) {
    const auto name_itr = _macro_aliases.find(macro);
    if (name_itr == _macro_aliases.end()) {
      return {};
    }

    return _parameters[name_itr->second];
  }

  String AttributeDB::get_macro_for_attribute(const String& class_name) {
    for (const auto& [macro, name] : _macro_aliases) {
      if (name == class_name) {
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

  bool AttributeDB::register_class_generator(const String& generator_name, Ref<ClassGenerator> generator) {
    if (_registered_generator_names.find(generator_name) == _registered_generator_names.end()) {
      _registered_generator_names.insert(generator_name);
      _class_generators.push_back(generator);
      return true;
    }
    return false;
  }

  const Vector<Ref<ClassGenerator>>& AttributeDB::class_generators() const { return _class_generators; }

}  // namespace GodotObjectCompiler
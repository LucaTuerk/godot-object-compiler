
#include "godot_property_hint.h"

#include "../../library/type_db.h"
#include "../assumptions.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler {

  String GodotPropertyHintParameterType::get_return_type() { return "GOC_PropertyHint"; }

  Vector<String> GodotPropertyHintParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotPropertyHintParameterType::get_arguments() {
    return {{ARG_STRING, "hint_string", true}};
  }

  bool GodotPropertyHintParameterType::get_godot_hint_for_value_name(const String& name, String& macro) {
    _value_names_lazy.poke();

    if (auto itr = _godot_hint_types.find(name); itr != _godot_hint_types.end()) {
      macro = itr->second;
      return true;
    }
    macro = "";
    return false;
  }

  Ref<Argument> GodotPropertyHintParameterType::create_argument() { return node_new<GodotPropertyHintArgument>(); }

  Vector<String> GodotPropertyHintParameterType::_value_names_lazy_get() {
    Ref<Enum> prop_hint_enum = TypeDB::instance()->get_type_data<Enum>(AssumedGodotTypes::PropertyHintEnum().type_name);
    if (!prop_hint_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& enum_value : prop_hint_enum->value_names()) {
      auto value_name = macro_case_to_pascal_case(string_replace(enum_value, "PROPERTY_", ""));
      _godot_hint_types[value_name] = enum_value;
      return_value.push_back(value_name);
    }
    return return_value;
  }

  String GodotPropertyHintArgument::_godot_property_hint_lazy_get() {
    auto ptype = GodotPropertyHintParameterType::instance();
    String hint;
    if (Ref<Identifier> identifier = find_child<Identifier>();
        identifier && ptype->get_godot_hint_for_value_name(identifier->name, hint)) {
      return hint;
    }
    PANIC("Malformed property hint argument");
  }

  String GodotPropertyHintArgument::_hint_string_lazy_get() {
    Ref<Arguments> arguments = find_child<Arguments>();
    if (!arguments) {
      return "\"\"";
    }

    Ref<Argument> argument = arguments->find_child<Argument>();
    if (!argument) {
      return "\"\"";
    }

    Ref<Literal> literal = argument->find_child<Literal>();
    if (!literal) {
      return "\"\"";
    }

    return literal->content;
  }

}  // namespace GodotObjectCompiler

#include "godot_variant_type.h"

#include "../assumptions.h"
#include "library/tree/syntax/enum.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  String GodotVariantTypeParameterType::get_return_type() { return "GOC_VariantType"; }

  Vector<String> GodotVariantTypeParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotVariantTypeParameterType::get_arguments() { return {}; }

  Ref<Argument> GodotVariantTypeParameterType::create_argument() { return node_new<GodotVariantTypeArgument>(); }

  bool GodotVariantTypeParameterType::get_variant_type_for_value_name(const String& value_name, String& variant_type) {
    _value_names_lazy.poke();

    auto itr = _value_name_to_godot_variant_type.find(value_name);
    if (itr == _value_name_to_godot_variant_type.end()) {
      variant_type = "";
      return false;
    }

    variant_type = itr->second;
    return true;
  }

  Vector<String> GodotVariantTypeParameterType::_value_names_lazy_get() {
    Ref<Enum> variant_type_enum =
        TypeDB::instance()->get_type_data<Enum>(AssumedGodotTypes::VariantTypeEnum().qualified_name);
    if (!variant_type_enum) {
      return {};
    }

    Vector<String> results;
    for (const String& enum_value : variant_type_enum->value_names()) {
      String value_name = macro_case_to_pascal_case("VARIANT_TYPE_" + enum_value);
      results.push_back(value_name);
      _value_name_to_godot_variant_type[value_name] = enum_value;
    }

    return results;
  }

  String GodotVariantTypeArgument::_godot_variant_type_lazy_get() {
    auto ptype = GodotVariantTypeParameterType::instance();

    Ref<Identifier> identifier = find_child<Identifier>();
    if (!identifier) {
      PANIC("Malformed GodotVariantTypeArgument");
    }

    String variant_type;
    if (!ptype->get_variant_type_for_value_name(identifier->name, variant_type)) {
      PANIC("Malformed GodotVariantTypeArgument");
    }

    return variant_type;
  }

}  // namespace GodotObjectCompiler

#include "library_godot/generators/godot_generator_utils.h"

#include "library/core/resources.h"
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_variant_type.h"

namespace GodotObjectCompiler {

  static bool _variant_types_initialized = false;
  static Dictionary<String, String> _type_to_variant_type;
  static Dictionary<String, Vector<String>> _variant_type_to_types;

  void ensure_type_dicts_initialized() {
    if (_variant_types_initialized) {
      return;
    }
    _variant_types_initialized = true;

    Ref<GodotVariantTypeParameterType> ptype = GodotVariantTypeParameterType::instance();

    for (const String& value_name : ptype->value_names()) {
      String res_path = "res://" + path_concat_ext("variant_types", value_name, "txt");
      if (Resources::instance()->has_resource(res_path)) {
        String content = Resources::instance()->load_text_resource(res_path);
        for (const String& line : string_split(content, "\n")) {
          String trimmed = string_trim(line);
          if (trimmed.length() == 0) {
            continue;
          }

          _type_to_variant_type[trimmed] = value_name;
          _variant_type_to_types[value_name].push_back(trimmed);
        }
      }
    }
  }

  Ref<Type> GodotGeneratorUtils::const_ref(const String& type_name) {
    return build<Type>().with_children({build<Const>(), build<Identifier>(type_name), build<Reference>()});
  }

  Ref<Function> GodotGeneratorUtils::bind_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Ref<Arguments> arguments;

    Ref<Function> result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_method"),
    build<Arguments>().with_children({
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(Writer::StringLiteral(method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(class_name),
        Writer::Text("::"),
        Writer::Text(method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : parameter_names) {
      arguments->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    return result;
    // clang-format on
  }

  Ref<Function> GodotGeneratorUtils::bind_static_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Ref<Arguments> arguments = node_new<Arguments>();

    Ref<Function> result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_static_method"),
    build<Arguments>().with_children({
      build<Argument>().with_child(Writer::StringLiteral(class_name)),
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(Writer::StringLiteral(method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(class_name),
        Writer::Text("::"),
        Writer::Text(method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : parameter_names) {
      arguments->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    return result;
    // clang-format on
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_bind_methods_body(
      const Ref<Class>& target_class, const Ref<Context>& generated_body, const Ref<Context>& generated_sources) {
    Ref<Function> bind_methods = generated_sources->find_child(
        0, NamedContextPredicates::name<Function>((target_class->name() + "::_bind_methods").c_str()));
    Ref<Body> bind_methods_body;

    if (!bind_methods) {
      // clang-format off
      generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(target_class->name() + "::" + "_bind_methods"),
      build<Parameters>(),
      build_ref<Body>(&bind_methods_body)}
      );

      generated_body->build_child<Function>().with_children({
        build<Static>(),
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>("_bind_methods"),
        build<Parameters>()
      }).with_child(Writer::Semicolon());
      // clang-format on
    } else {
      bind_methods_body = bind_methods->find_child<Body>();
    }
    return bind_methods_body;
  }

  bool GodotGeneratorUtils::check_is_valid_named_argument(const Ref<Node>& node, String& name) {
    if (node == nullptr) {
      return false;
    }

    name = "";
    return !tree_has_error(node) && get_name(node, name);
  }

  bool GodotGeneratorUtils::get_name(const Ref<Node>& node, String& name) {
    name = "";
    if (node == nullptr) {
      return false;
    }

    if (Ref<Identifier> id = node->as<Identifier>()) {
      name = id->name;
      return true;
    }

    if (Ref<Context> context = node->as<Context>()) {
      for (const Ref<Node>& child : context->get_children()) {
        if (Ref<Identifier> id = child->as<Identifier>()) {
          name = id->name;
          return true;
        }
      }
    }

    return false;
  }

  bool GodotGeneratorUtils::tree_has_error(const Ref<Node>& tree) {
    if (tree == nullptr) {
      return false;
    }
    if (tree->is<Error>()) {
      return true;
    }

    if (Ref<Context> context = tree->as<Context>()) {
      for (const Ref<Node>& child : *context) {
        if (tree_has_error(child)) {
          return true;
        }
      }
    }

    return false;
  }

  bool GodotGeneratorUtils::class_has_base_class(const Ref<Class>& target_class, const String& base_class_qualified) {
    if (target_class->qualified_name() == base_class_qualified) {
      return true;
    }

    // TODO: this will not work if the base class name is not fully qualified
    for (const String& base : target_class->direct_bases_names()) {
      Ref<Class> base_class = TypeDB::instance()->get_type_data<Class>(base);
      if (!base_class) {
        print_err("Base class not found!");
        return false;
      }

      return class_has_base_class(base_class, base_class_qualified);
    }

    return false;
  }

  bool GodotGeneratorUtils::class_is_node_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::Node().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_resource_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::Resource().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_ref_counted_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::RefCounted().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_godot_object_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::Object().qualified_name);
  }

  bool GodotGeneratorUtils::get_defaults_for_type(const Ref<Type>& target_type,
      Ref<GodotVariantTypeArgument>& variant_type, Ref<GodotPropertyHintArgument>& property_hint,
      Ref<GodotPropertyUsageFlagsArgument>& property_usage_flags, DefaultsUsage usage) {
    using namespace AssumedParameterValues;
    if (Ref<Type> ref_inner_type;
        type_is_godot_ref_type(target_type, ref_inner_type) && type_is_ref_counted_type(ref_inner_type)) {
      variant_type = VariantType(VariantTypeObject());
      property_hint = PropertyHint(HintResourceType(), ref_inner_type->name());
      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    if (type_is_node_type(target_type)) {
      variant_type = VariantType(VariantTypeObject());
      property_hint = PropertyHint(HintNodeType(), target_type->name());
      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    if (type_is_object_type(target_type)) {
      variant_type = VariantType(VariantTypeObject());
      property_hint = PropertyHint(HintResourceType(), target_type->name());
      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    if (Ref<Enum> enum_object; type_is_enum_type(target_type,enum_object)) {
      variant_type = VariantType(VariantTypeInt());
      Vector<String> value_names = enum_object->value_names();
      std::transform(value_names.begin(), value_names.end(), value_names.begin(),cpp_enum_case_to_exposed_enum_case);
      property_hint = PropertyHint(HintEnum(), string_vector_combine(value_names,","));
      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    if (Ref<Type> inner_type; type_is_godot_typed_array_type(target_type, inner_type)) {
      Ref<GodotVariantTypeArgument> inner_variant_type;
      Ref<GodotPropertyHintArgument> inner_property_hint;
      Ref<GodotPropertyUsageFlagsArgument> _;

      if (!get_defaults_for_type(inner_type, inner_variant_type, inner_property_hint, _)) {
        return false;
      }

      variant_type = VariantType(VariantTypeArray());

      switch (usage) {
        case DEFAULTS_PROPERTY_BINDING: {
          property_hint = PropertyHint(HintArrayType(),
              "vformat(\"%s/%s:%s\", " +
                  format("%s,%s,%s)", ("Variant::" + inner_variant_type->godot_variant_type()).c_str(),
                      inner_property_hint->godot_property_hint().c_str(), inner_property_hint->hint_string().c_str()),
              false);
        } break;
        case DEFAULTS_SIGNAL_ARGUMENT: {
          property_hint = PropertyHint(HintArrayType(), inner_type->name(), true);
        } break;
      }

      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    if (Ref<Type> key_type, value_type; type_is_godot_typed_dictionary_type(target_type, key_type, value_type)) {
      Ref<GodotVariantTypeArgument> key_variant_type;
      Ref<GodotPropertyHintArgument> key_property_hint;
      Ref<GodotVariantTypeArgument> value_variant_type;
      Ref<GodotPropertyHintArgument> value_property_hint;
      Ref<GodotPropertyUsageFlagsArgument> _;

      if (!get_defaults_for_type(key_type, key_variant_type, key_property_hint, _) ||
          !get_defaults_for_type(value_type, value_variant_type, value_property_hint, _)) {
        return false;
      }
      variant_type = VariantType(VariantTypeDictionary());
      property_usage_flags = PropertyUsageFlag(UsageDefault());

      switch (usage) {
        case DEFAULTS_PROPERTY_BINDING: {
          property_hint = PropertyHint(HintDictionaryType(),
              "vformat(\"%s/%s:%s;%s/%s:%s\", " +
                  format("%s,%s,%s,%s,%s,%s)", ("Variant::" + key_variant_type->godot_variant_type()).c_str(),
                      key_property_hint->godot_property_hint().c_str(), key_property_hint->hint_string().c_str(),
                      ("Variant::" + value_variant_type->godot_variant_type()).c_str(),
                      value_property_hint->godot_property_hint().c_str(), value_property_hint->hint_string().c_str()),
              false);
        } break;
        case DEFAULTS_SIGNAL_ARGUMENT: {
          property_hint = PropertyHint(
              HintDictionaryType(), format("%s;%s", key_type->name().c_str(), value_type->name().c_str()), true);
        } break;
      }

      return true;
    }

    if (String variant_type_name; get_variant_type_from_type(target_type, variant_type_name)) {
      variant_type = VariantType(variant_type_name);
      property_hint = PropertyHint(HintNone());
      property_usage_flags = PropertyUsageFlag(UsageDefault());
      return true;
    }

    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_ref_type(const Ref<Type>& target_type, Ref<Type>& inner_type) {
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(target_type, AssumedGodotTypes::GodotRef(), inner_types)) {
      inner_type = inner_types[0];
      return true;
    }
    inner_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_typed_array_type(const Ref<Type>& target_type, Ref<Type>& inner_type) {
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(target_type, AssumedGodotTypes::TypedArray(), inner_types)) {
      inner_type = inner_types[0];
      return true;
    }
    inner_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_typed_dictionary_type(
      const Ref<Type>& target_type, Ref<Type>& key_type, Ref<Type>& value_type) {
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(target_type, AssumedGodotTypes::TypedDictionary(), inner_types)) {
      key_type = inner_types[0];
      value_type = inner_types[1];
      return true;
    }
    key_type = nullptr;
    value_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_ref_counted_type(const Ref<Type>& inner_type) {
    const Ref<Class> _class = TypeDB::instance()->get_type_data<Class>(inner_type->type_name_unmodified());
    if (!_class) {
      return false;
    }

    return class_is_ref_counted_type(_class);
  }

  bool GodotGeneratorUtils::type_is_object_type(const Ref<Type>& target_type) {
    const Ref<Class> _class = TypeDB::instance()->get_type_data<Class>(target_type->type_name_unmodified());
    if (!_class) {
      return false;
    }

    return class_is_godot_object_type(_class);
  }

  bool GodotGeneratorUtils::type_is_godot_collection_type(const Ref<Type>& target_type) {
    Vector<Ref<Type>> inner_types;
    return target_type->type_name_unmodified() == AssumedGodotTypes::Array().qualified_name ||
           target_type->type_name_unmodified() == AssumedGodotTypes::Dictionary().qualified_name ||
           type_is_assumed_template_type(target_type, AssumedGodotTypes::TypedArray(), inner_types) ||
           type_is_assumed_template_type(target_type, AssumedGodotTypes::TypedDictionary(), inner_types);
  }

  bool GodotGeneratorUtils::type_is_node_type(const Ref<Type>& target_type) {
    const Ref<Class> _class = TypeDB::instance()->get_type_data<Class>(target_type->type_name_unmodified());
    if (!_class) {
      return false;
    }

    return class_is_node_type(_class);
  }

  bool GodotGeneratorUtils::type_is_enum_type(const Ref<Type>& target_type, Ref<Enum>& enum_object) {
    const Ref<Enum> _enum = TypeDB::instance()->get_type_data<Enum>(target_type->type_name_unmodified());
    if (!_enum) {
      enum_object = nullptr;
      return false;
    }
    enum_object = _enum;
    return true;
  }

  bool GodotGeneratorUtils::type_is_variant_type(const Ref<Type>& target_type) {
    ensure_type_dicts_initialized();
    return _type_to_variant_type.find(target_type->type_name_unmodified()) != _type_to_variant_type.end();
  }

  bool GodotGeneratorUtils::get_variant_type_from_type(const Ref<Type>& target_type, String& variant_type) {
    ensure_type_dicts_initialized();
    if (auto itr = _type_to_variant_type.find(target_type->type_name_unmodified());
        itr != _type_to_variant_type.end()) {
      variant_type = itr->second;
      return true;
    }

    variant_type = "";
    return false;
  }

  Ref<GodotVariantTypeArgument> GodotGeneratorUtils::VariantTypeFromType(const Ref<Type>& type) {
    Ref<Type> inner_type;
    if (type_is_object_type(type) || type_is_godot_ref_type(type, inner_type)) {
      return VariantType(AssumedParameterValues::VariantTypeObject());
    } else if (String variant_type; get_variant_type_from_type(type, variant_type)) {
      return VariantType(variant_type);
    }

    return VariantType(AssumedParameterValues::VariantTypeNil());
  }

  Ref<GodotVariantTypeArgument> GodotGeneratorUtils::VariantType(const String& variant_type) {
    return build<GodotVariantTypeArgument>().with_child<Identifier>(variant_type);
  }

  Ref<GodotPropertyUsageFlagsArgument> GodotGeneratorUtils::PropertyUsageFlag(const String& usage) {
    return build<GodotPropertyUsageFlagsArgument>().with_child<Identifier>(usage);
  }

  Ref<Node> GodotGeneratorUtils::PropertyInfo(const Ref<GodotVariantTypeArgument>& variant_type,
      const Ref<GodotPropertyHintArgument>& hint, const Vector<Ref<GodotPropertyUsageFlagsArgument>>& usages,
      const String& property_name) {
    Ref<Arguments> arguments;
    Ref<Writer::ListNode> flags;
    // clang-format off
    Ref<Node> result = build<Function>().with_children({
      build<Identifier>("PropertyInfo"),
      build_ref<Arguments>(&arguments).with_children({
        build<Argument>().with_child<Literal>("Variant::" + variant_type->godot_variant_type()),
        build<Argument>().with_child(Literal::StringLiteral(property_name)),
          build<Argument>().with_child<Literal>(hint->godot_property_hint()),
        build<Argument>().with_child<Literal>(hint->hint_string()),
        build_ref<Writer::ListNode>(&flags," | ", false, false)
      })});
    // clang-format on

    if (usages.size() == 0) {
      arguments->remove_child(flags);
    } else {
      for (const Ref<GodotPropertyUsageFlagsArgument>& usage : usages) {
        flags->add_child(build<Literal>(usage->godot_property_usage_flag()));
      }
    }
    return result;
  }

  Ref<Node> GodotGeneratorUtils::PropertyInfo(
      const Ref<GodotVariantTypeArgument>& variant_type, const String& property_name) {
    // clang-format off
    return build<Function>().with_children({
      build<Identifier>("PropertyInfo"),
      build<Arguments>().with_children({
        build<Argument>().with_child<Literal>("Variant::" + variant_type->godot_variant_type()),
        build<Argument>().with_child(Literal::StringLiteral(property_name)),
      })});
    // clang-format on
  }

  Ref<Node> GodotGeneratorUtils::PropertyInfoDefaultForType(
      const Ref<Type>& type, const String& property_name, DefaultsUsage usage) {
    Ref<GodotVariantTypeArgument> variant_type;
    Ref<GodotPropertyHintArgument> property_hint;
    Ref<GodotPropertyUsageFlagsArgument> usage_flags;

    if (!get_defaults_for_type(type, variant_type, property_hint, usage_flags, usage)) {
      return nullptr;
    }

    return PropertyInfo(variant_type, property_hint, {usage_flags}, property_name);
  }

  Ref<GodotPropertyHintArgument> GodotGeneratorUtils::PropertyHint(
      const String& value, const String& hint_string, bool is_string_literal) {
    // clang-format off
    return build<GodotPropertyHintArgument>().with_children({
      build<Identifier>(value),
      build<Arguments>().with_children({
          build<Argument>().with_child( is_string_literal ? Literal::StringLiteral(hint_string) : node_new<Literal>(hint_string)),
      })
    });
    // clang-format off
  }


}  // namespace GodotObjectCompiler

#include "library_godot/generators/godot_generator_utils.h"

#include "../../library/type_db.h"
#include "../assumptions.h"
#include "library/core/resources.h"
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"
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
      String res_path = path_concat_ext("res://variant_types", value_name, "txt");
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
    return class_has_base_class(target_class, AssumedGodotTypes::Node().type_name);
  }

  bool GodotGeneratorUtils::class_is_resource_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::Resource().type_name);
  }

  bool GodotGeneratorUtils::class_is_ref_counted_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::RefCounted().type_name);
  }

  bool GodotGeneratorUtils::class_is_godot_object_type(const Ref<Class>& target_class) {
    return class_has_base_class(target_class, AssumedGodotTypes::Object().type_name);
  }

  bool GodotGeneratorUtils::type_is_godot_ref_type(const Ref<Type>& target_type, Ref<Type>& inner_type) {
    if (target_type->type_name() != AssumedGodotTypes::GodotRef().type_name) {
      return false;
    }

    if (target_type->template_argument_count() != AssumedGodotTypes::GodotRef().template_arg_count) {
      return false;
    }

    Ref<TemplateArguments> template_arguments = target_type->template_arguments();
    inner_type = template_arguments->find_child<Type>();
    if (!inner_type) {
      return false;
    }

    return true;
  }

  bool GodotGeneratorUtils::type_is_ref_counted_type(const Ref<Type>& target_type) {
    const Ref<Class> _class = TypeDB::instance()->get_type_data<Class>(target_type->type_name_unmodified());
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

  bool GodotGeneratorUtils::type_is_node_type(const Ref<Type>& target_type) {
    const Ref<Class> _class = TypeDB::instance()->get_type_data<Class>(target_type->type_name_unmodified());
    if (!_class) {
      return false;
    }

    return class_is_node_type(_class);
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

  Ref<GodotPropertyHintArgument> GodotGeneratorUtils::PropertyHint(const String& value, const String& hint_string) {
    // clang-format off
    return build<GodotPropertyHintArgument>().with_children({
      build<Identifier>(value),
      build<Arguments>().with_children({
          build<Argument>().with_child(Literal::StringLiteral(hint_string)),
      })
    });
    // clang-format off
  }

}  // namespace GodotObjectCompiler
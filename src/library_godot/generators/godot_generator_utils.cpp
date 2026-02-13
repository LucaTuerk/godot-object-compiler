/**************************************************************************/
/* godot_generator_utils.cpp                                              */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "library_godot/generators/godot_generator_utils.h"

#include "library/core/file_system_utilities.h"
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
#include "library_godot/attributes/godot_attributes.h"
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

    const Ref<GodotVariantTypeParameterType> ptype = GodotVariantTypeParameterType::instance();

    for (const String& value_name : ptype->value_names()) {
      String res_path = "res://" + path_concat_ext("variant_types", value_name, "txt");
      if (Resources::instance()->has_resource(res_path)) {
        String content = Resources::instance()->load_text_resource(res_path);
        for (const String& line : string_split(content, "\n")) {
          String trimmed = string_trim(line);
          if (trimmed.empty()) {
            continue;
          }

          _type_to_variant_type[trimmed] = value_name;
          _variant_type_to_types[value_name].push_back(trimmed);
        }
      }
    }
  }

  Ref<Type> GodotGeneratorUtils::const_ref(const String& p_type_name) {
    return build<Type>().with_children({build<Const>(), build<Identifier>(p_type_name), build<Reference>()});
  }

  Ref<Function> GodotGeneratorUtils::bind_method(const String& p_class_name, const String& p_method_name,
      const Vector<String>& p_parameter_names, const Vector<String>& p_default_values) {
    // clang-format off
    Ref<Arguments> bind_args;
    Ref<Arguments> d_method_args;

    Ref<Function> result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_method"),
    build_ref<Arguments>(&bind_args).with_children({
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&d_method_args).with_children({
            build<Argument>().with_child(Writer::StringLiteral(p_method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(p_class_name),
        Writer::Text("::"),
        Writer::Text(p_method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : p_parameter_names) {
      d_method_args->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    for (const String& def_val : p_default_values) {
      bind_args->build_child<Argument>().with_child(
        Writer::Text(format("DEFVAL(%s)", def_val.c_str())
      ));
    }

    return result;
    // clang-format on
  }

  Ref<Function> GodotGeneratorUtils::bind_static_method(const String& p_class_name, const String& p_method_name,
      const Vector<String>& p_parameter_names, const Vector<String>& p_default_values) {
    // clang-format off
    Ref<Arguments> bind_args;
    Ref<Arguments> d_method_args;

    Ref<Function> result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_static_method"),
    build_ref<Arguments>(&bind_args).with_children({
      build<Argument>().with_child(Writer::StringLiteral(p_class_name)),
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&d_method_args).with_children({
            build<Argument>().with_child(Writer::StringLiteral(p_method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(p_class_name),
        Writer::Text("::"),
        Writer::Text(p_method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : p_parameter_names) {
      d_method_args->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    for (const String& def_val : p_default_values) {
      bind_args->build_child<Argument>().with_child(
        Writer::Text(format("DEFVAL(%s)", def_val.c_str())
      ));
    }

    return result;
    // clang-format on
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_bind_methods_body(
      const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body, const Ref<Context>& p_generated_sources) {
    bool bind_methods_defined = p_target_class->has_function_named("_bind_methods");
    const String bind_methods_name = bind_methods_defined ? "_generated_bind_methods" : "_bind_methods";
    const String qualified_bind_methods_name =
        format("%s::%s", p_target_class->name().c_str(), bind_methods_name.c_str());

    Ref<Function> bind_methods =
        p_generated_sources->find_child(0, NamedContextPredicates::name<Function>(qualified_bind_methods_name.c_str()));
    Ref<Body> bind_methods_body;

    if (!bind_methods) {
      // clang-format off
      p_generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(qualified_bind_methods_name),
      build<Parameters>(),
      build_ref<Body>(&bind_methods_body)}
      );

      p_generated_body->build_child<Function>().with_children({
        build<Static>(),
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(bind_methods_name),
        build<Parameters>()
      }).with_child(Writer::Semicolon());
      // clang-format on
    } else {
      bind_methods_body = bind_methods->find_child<Body>();
      PANIC_COND(!bind_methods_body, "Body not found.");
    }
    return bind_methods_body;
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_get_property_list_body(
      const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body, const Ref<Context>& p_generated_sources) {
    bool get_property_list_defined = p_target_class->has_function_named("_get_property_list");
    const String property_list_name = get_property_list_defined ? "_generated_get_property_list" : "_get_property_list";
    const String qualified_property_list_name =
        format("%s::%s", p_target_class->name().c_str(), property_list_name.c_str());

    Ref<Function> get_property_list = p_generated_sources->find_child(
        0, NamedContextPredicates::name<Function>(qualified_property_list_name.c_str()));
    Ref<Body> get_property_list_body;

    if (!get_property_list) {
      // clang-format off
      p_generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(qualified_property_list_name),
      build<Parameters>().with_children({
        build<Parameter>().with_children({
         build<Type>().with_children({
            build<Identifier>("List<PropertyInfo>"),
           build<Pointer>()
            }),
        build<Identifier>("p_list")
          })
        }),
        build<Const>(),
        build_ref<Body>(&get_property_list_body)}
      );

      p_generated_body->build_child<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(property_list_name),
        build<Parameters>().with_children({
          build<Parameter>().with_children({
           build<Type>().with_children({
              build<Identifier>("List<PropertyInfo>"),
             build<Pointer>()
              }),
          build<Identifier>("p_list")
            })
          }),
        build<Const>(),
      }).with_child(Writer::Semicolon());
      // clang-format on
    } else {
      get_property_list_body = get_property_list->find_child<Body>();
      PANIC_COND(!get_property_list_body, "Body not found.");
    }

    return get_property_list_body;
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_function_names_body(
      const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body) {
    Ref<Struct> function_names = p_generated_body->find_child(0, NamedContextPredicates::name<Struct>("FunctionNames"));
    Ref<Body> function_names_body;

    if (!function_names) {
      // clang-format off
      auto base_names = p_target_class->direct_bases_names();
      if (base_names.size() == 1 && ExecutionContext::instance()->get_type_db()->get_type_attribute(base_names[0],GodotClassAttribute::get_type_static(), 0, p_target_class) != nullptr) {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("FunctionNames"),
          build<BaseClasses>().with_child(
            build<Type>().with_child<Identifier>(base_names[0] + "::FunctionNames")
          ),
          build_ref<Body>(&function_names_body)
        });
      } else {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("FunctionNames"),
          build_ref<Body>(&function_names_body)
        });
      }
      // clang-format on
    } else {
      function_names_body = function_names->find_child<Body>();
      PANIC_COND(!function_names_body, "Body not found.");
    }

    return function_names_body;
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_property_names_body(
      const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body) {
    Ref<Struct> property_names = p_generated_body->find_child(0, NamedContextPredicates::name<Struct>("PropertyNames"));
    Ref<Body> property_names_body;
    if (!property_names) {
      // clang-format off
      auto base_names = p_target_class->direct_bases_names();
      if (base_names.size() == 1 && ExecutionContext::instance()->get_type_db()->get_type_attribute(base_names[0],GodotClassAttribute::get_type_static(), 0, p_target_class) != nullptr) {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("PropertyNames"),
          build<BaseClasses>().with_child(
            build<Type>().with_child<Identifier>(base_names[0] + "::PropertyNames")
          ),
          build_ref<Body>(&property_names_body)
        });
      } else {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("PropertyNames"),
          build_ref<Body>(&property_names_body)
        });
      }
      // clang-format on
    } else {
      property_names_body = property_names->find_child<Body>();
      PANIC_COND(!property_names_body, "Body not found.");
    }

    return property_names_body;
  }

  Ref<Body> GodotGeneratorUtils::get_or_create_signal_names_body(
      const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body) {
    Ref<Struct> signal_names = p_generated_body->find_child(0, NamedContextPredicates::name<Struct>("SignalNames"));
    Ref<Body> signal_names_body;
    if (!signal_names) {
      // clang-format off
      auto base_names = p_target_class->direct_bases_names();
      if (base_names.size() == 1 && ExecutionContext::instance()->get_type_db()->get_type_attribute(base_names[0],GodotClassAttribute::get_type_static(), 0, p_target_class) != nullptr) {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("SignalNames"),
          build<BaseClasses>().with_child(
            build<Type>().with_child<Identifier>(base_names[0] + "::SignalNames")
          ),
          build_ref<Body>(&signal_names_body)
        });
      } else {
        p_generated_body->build_child<Struct>().with_children({
          build<Identifier>("SignalNames"),
          build_ref<Body>(&signal_names_body)
        });
      }
      // clang-format on
    } else {
      signal_names_body = signal_names->find_child<Body>();
      PANIC_COND(!signal_names_body, "Body not found.");
    }

    return signal_names_body;
  }

  bool GodotGeneratorUtils::check_is_valid_named_argument(const Ref<Node>& p_node, String& p_name) {
    if (p_node == nullptr) {
      return false;
    }

    p_name = "";
    return !tree_has_error(p_node) && get_name(p_node, p_name);
  }

  bool GodotGeneratorUtils::get_name(const Ref<Node>& p_node, String& p_name) {
    p_name = "";
    if (p_node == nullptr) {
      return false;
    }

    if (Ref<Identifier> id = p_node->as<Identifier>()) {
      p_name = id->name;
      return true;
    }

    if (Ref<Context> context = p_node->as<Context>()) {
      for (const Ref<Node>& child : context->get_children()) {
        if (Ref<Identifier> id = child->as<Identifier>()) {
          p_name = id->name;
          return true;
        }
      }
    }

    return false;
  }

  bool GodotGeneratorUtils::tree_has_error(const Ref<Node>& p_tree) {
    if (p_tree == nullptr) {
      return false;
    }
    if (p_tree->is<Error>()) {
      return true;
    }

    if (Ref<Context> context = p_tree->as<Context>()) {
      for (const Ref<Node>& child : *context) {
        if (tree_has_error(child)) {
          return true;
        }
      }
    }

    return false;
  }

  bool GodotGeneratorUtils::class_has_base_class(
      const Ref<Class>& p_target_class, const String& p_base_class_qualified) {
    if (p_target_class->qualified_name() == p_base_class_qualified ||
        p_target_class->qualified_name() == "godot::" + p_base_class_qualified) {
      return true;
    }

    // TODO: this will not work if the base class name is not fully qualified
    for (const String& base : p_target_class->direct_bases_names()) {
      Ref<Class> base_class = ExecutionContext::instance()->get_type_db()->get_type_data<Class>(base);
      if (!base_class) {
        print_err("Base class not found!");
        return false;
      }

      return class_has_base_class(base_class, p_base_class_qualified);
    }

    return false;
  }

  bool GodotGeneratorUtils::class_is_node_type(const Ref<Class>& p_target_class) {
    return class_has_base_class(p_target_class, AssumedGodotTypes::Node().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_resource_type(const Ref<Class>& p_target_class) {
    return class_has_base_class(p_target_class, AssumedGodotTypes::Resource().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_ref_counted_type(const Ref<Class>& p_target_class) {
    return class_has_base_class(p_target_class, AssumedGodotTypes::RefCounted().qualified_name);
  }

  bool GodotGeneratorUtils::class_is_godot_object_type(const Ref<Class>& p_target_class) {
    return class_has_base_class(p_target_class, AssumedGodotTypes::Object().qualified_name);
  }

  bool GodotGeneratorUtils::get_defaults_for_type(const Ref<Type>& p_target_type,
      Ref<GodotVariantTypeArgument>& p_variant_type, Ref<GodotPropertyHintArgument>& p_property_hint,
      Ref<GodotPropertyUsageFlagsArgument>& p_property_usage_flags, const Ref<Namespace>& p_from_namespace,
      DefaultsUsage p_defaults_usage) {
    using namespace AssumedParameterValues;
    if (Ref<Type> ref_inner_type; type_is_godot_ref_type(p_target_type, ref_inner_type, p_from_namespace) &&
        type_is_ref_counted_type(ref_inner_type, p_from_namespace)) {
      p_variant_type = build_variant_type_argument(VariantTypeObject());
      p_property_hint = build_property_hint_argument(HintResourceType(), ref_inner_type->name());
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
      return true;
    }

    if (type_is_node_type(p_target_type, p_from_namespace)) {
      p_variant_type = build_variant_type_argument(VariantTypeObject());
      p_property_hint = build_property_hint_argument(HintNodeType(), p_target_type->name());
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
      return true;
    }

    if (type_is_object_type(p_target_type)) {
      p_variant_type = build_variant_type_argument(VariantTypeObject());
      p_property_hint = build_property_hint_argument(HintResourceType(), p_target_type->name());
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
      return true;
    }

    if (Ref<Enum> enum_object; type_is_enum_type(p_target_type, enum_object, p_from_namespace)) {
      p_variant_type = build_variant_type_argument(VariantTypeInt());
      Vector<String> hints;
      for (const Ref<EnumValue>& value : enum_object->values()) {
        hints.push_back(format(
            "%s:%s", cpp_enum_case_to_exposed_enum_case(value->name()).c_str(), value->literal()->content.c_str()));
      }

      Ref<GodotEnumAttribute> attribute =
          ExecutionContext::instance()->get_type_db()->get_type_attribute<GodotEnumAttribute>(
              p_target_type, p_from_namespace);

      bool is_flags = false;

      if (attribute) {
        if (const Ref<Identifier> argument_identifier =
                attribute->find_chain<Identifier, Arguments, EnumGeneratorOptionsArgument>()) {
          is_flags = argument_identifier->name == EnumGeneratorOptionsArgument::EnumFlags;
        }
      }

      if (is_flags) {
        p_property_hint = build_property_hint_argument(HintFlags(), string_vector_combine(hints, ","));
      } else {
        p_property_hint = build_property_hint_argument(HintEnum(), string_vector_combine(hints, ","));
      }
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());

      return true;
    }

    if (Ref<Type> inner_type; type_is_godot_typed_array_type(p_target_type, inner_type, p_from_namespace)) {
      Ref<GodotVariantTypeArgument> inner_variant_type;
      Ref<GodotPropertyHintArgument> inner_property_hint;
      Ref<GodotPropertyUsageFlagsArgument> _;

      if (!get_defaults_for_type(inner_type, inner_variant_type, inner_property_hint, _, p_from_namespace)) {
        return false;
      }

      p_variant_type = build_variant_type_argument(VariantTypeArray());

      switch (p_defaults_usage) {
        case DEFAULTS_PROPERTY_BINDING: {
          p_property_hint = build_property_hint_argument(HintArrayType(),
              "vformat(\"%s/%s:%s\", " +
                  format("%s,%s,%s)", ("Variant::" + inner_variant_type->godot_variant_type()).c_str(),
                      inner_property_hint->godot_property_hint().c_str(), inner_property_hint->hint_string().c_str()),
              false);
        } break;
        case DEFAULTS_SIGNAL_ARGUMENT: {
          p_property_hint = build_property_hint_argument(HintArrayType(), inner_type->name(), true);
        } break;
      }

      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
      return true;
    }

    if (Ref<Type> key_type, value_type;
        type_is_godot_typed_dictionary_type(p_target_type, key_type, value_type, p_from_namespace)) {
      Ref<GodotVariantTypeArgument> key_variant_type;
      Ref<GodotPropertyHintArgument> key_property_hint;
      Ref<GodotVariantTypeArgument> value_variant_type;
      Ref<GodotPropertyHintArgument> value_property_hint;
      Ref<GodotPropertyUsageFlagsArgument> _;

      if (!get_defaults_for_type(key_type, key_variant_type, key_property_hint, _, p_from_namespace) ||
          !get_defaults_for_type(value_type, value_variant_type, value_property_hint, _, p_from_namespace)) {
        return false;
      }
      p_variant_type = build_variant_type_argument(VariantTypeDictionary());
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());

      switch (p_defaults_usage) {
        case DEFAULTS_PROPERTY_BINDING: {
          p_property_hint = build_property_hint_argument(HintDictionaryType(),
              "vformat(\"%s/%s:%s;%s/%s:%s\", " +
                  format("%s,%s,%s,%s,%s,%s)", ("Variant::" + key_variant_type->godot_variant_type()).c_str(),
                      key_property_hint->godot_property_hint().c_str(), key_property_hint->hint_string().c_str(),
                      ("Variant::" + value_variant_type->godot_variant_type()).c_str(),
                      value_property_hint->godot_property_hint().c_str(), value_property_hint->hint_string().c_str()),
              false);
        } break;
        case DEFAULTS_SIGNAL_ARGUMENT: {
          p_property_hint = build_property_hint_argument(
              HintDictionaryType(), format("%s;%s", key_type->name().c_str(), value_type->name().c_str()), true);
        } break;
      }

      return true;
    }

    if (String variant_type_name; get_variant_type_from_type(p_target_type, variant_type_name)) {
      p_variant_type = build_variant_type_argument(variant_type_name);
      p_property_hint = build_property_hint_argument(HintNone());
      p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
      return true;
    }

    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_ref_type(
      const Ref<Type>& p_target_type, Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace) {
    UNUSED(p_from_namespace);
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(p_target_type, AssumedGodotTypes::GodotRef(), inner_types)) {
      p_inner_type = inner_types[0];
      return true;
    }
    p_inner_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_typed_array_type(
      const Ref<Type>& p_target_type, Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace) {
    UNUSED(p_from_namespace);
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(p_target_type, AssumedGodotTypes::TypedArray(), inner_types)) {
      p_inner_type = inner_types[0];
      return true;
    }
    p_inner_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_godot_typed_dictionary_type(const Ref<Type>& p_target_type, Ref<Type>& p_key_type,
      Ref<Type>& p_value_type, const Ref<Namespace>& p_from_namespace) {
    UNUSED(p_from_namespace);
    Vector<Ref<Type>> inner_types;
    if (type_is_assumed_template_type(p_target_type, AssumedGodotTypes::TypedDictionary(), inner_types)) {
      p_key_type = inner_types[0];
      p_value_type = inner_types[1];
      return true;
    }
    p_key_type = nullptr;
    p_value_type = nullptr;
    return false;
  }

  bool GodotGeneratorUtils::type_is_ref_counted_type(
      const Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace) {
    const Ref<Class> _class = ExecutionContext::instance()->get_type_db()->get_type_data<Class>(
        p_inner_type->type_name_unmodified(), 0, p_from_namespace);
    if (!_class) {
      return false;
    }

    return class_is_ref_counted_type(_class);
  }

  bool GodotGeneratorUtils::type_is_object_type(
      const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace) {
    const Ref<Class> _class = ExecutionContext::instance()->get_type_db()->get_type_data<Class>(
        p_target_type->type_name_unmodified(), 0, p_from_namespace);
    if (!_class) {
      return false;
    }

    return class_is_godot_object_type(_class);
  }

  bool GodotGeneratorUtils::type_is_godot_collection_type(
      const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace) {
    UNUSED(p_from_namespace);
    Vector<Ref<Type>> inner_types;
    return p_target_type->type_name_unmodified() == AssumedGodotTypes::Array().qualified_name ||
        p_target_type->type_name_unmodified() == AssumedGodotTypes::Dictionary().qualified_name ||
        type_is_assumed_template_type(p_target_type, AssumedGodotTypes::TypedArray(), inner_types) ||
        type_is_assumed_template_type(p_target_type, AssumedGodotTypes::TypedDictionary(), inner_types);
  }

  bool GodotGeneratorUtils::type_is_node_type(const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace) {
    const Ref<Class> _class = ExecutionContext::instance()->get_type_db()->get_type_data<Class>(
        p_target_type->type_name_unmodified(), 0, p_from_namespace);
    if (!_class) {
      return false;
    }

    return class_is_node_type(_class);
  }

  bool GodotGeneratorUtils::type_is_enum_type(
      const Ref<Type>& p_target_type, Ref<Enum>& p_enum_object, const Ref<Namespace>& p_from_namespace) {
    const Ref<Enum> _enum = ExecutionContext::instance()->get_type_db()->get_type_data<Enum>(
        p_target_type->type_name_unmodified(), 0, p_from_namespace);
    if (!_enum) {
      p_enum_object = nullptr;
      return false;
    }
    p_enum_object = _enum;
    return true;
  }

  bool GodotGeneratorUtils::type_is_variant_type(
      const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace) {
    UNUSED(p_from_namespace);

    ensure_type_dicts_initialized();
    return _type_to_variant_type.find(p_target_type->type_name_unmodified()) != _type_to_variant_type.end();
  }

  bool GodotGeneratorUtils::get_variant_type_from_type(const Ref<Type>& p_target_type, String& p_variant_type) {
    ensure_type_dicts_initialized();
    if (auto itr = _type_to_variant_type.find(p_target_type->type_name_unmodified());
        itr != _type_to_variant_type.end()) {
      p_variant_type = itr->second;
      return true;
    }

    p_variant_type = "";
    return false;
  }

  Ref<GodotVariantTypeArgument> GodotGeneratorUtils::build_variant_type_argument(const Ref<Type>& p_type) {
    Ref<Type> inner_type;
    if (type_is_object_type(p_type) || type_is_godot_ref_type(p_type, inner_type)) {
      return build_variant_type_argument(AssumedParameterValues::VariantTypeObject());
    } else if (String variant_type; get_variant_type_from_type(p_type, variant_type)) {
      return build_variant_type_argument(variant_type);
    }

    return build_variant_type_argument(AssumedParameterValues::VariantTypeNil());
  }

  Ref<GodotVariantTypeArgument> GodotGeneratorUtils::build_variant_type_argument(const String& p_variant_type) {
    return build<GodotVariantTypeArgument>().with_child<Identifier>(p_variant_type);
  }

  Ref<GodotPropertyUsageFlagsArgument> GodotGeneratorUtils::build_property_usage_flags_argument(const String& p_usage) {
    return build<GodotPropertyUsageFlagsArgument>().with_child<Identifier>(p_usage);
  }

  Ref<Node> GodotGeneratorUtils::build_property_info(const Ref<GodotVariantTypeArgument>& p_variant_type,
      const Ref<GodotPropertyHintArgument>& p_hint, const Vector<Ref<GodotPropertyUsageFlagsArgument>>& p_usages,
      const String& p_property_name, bool p_no_editor) {
    Ref<Arguments> arguments;
    Ref<Writer::ListNode> flags;
    // clang-format off
    Ref<Node> property_usage;
    if (p_no_editor) {
      property_usage = build<Writer::ListNode>(" ", false, false).with_children({
        build<Writer::EnclosingNode>("(",")").with_children({
          build_ref<Writer::ListNode>(&flags," | ", false, false)
        }),
        Writer::Text("& ~PROPERTY_USAGE_EDITOR"),
      });
    } else {
      property_usage = build_ref<Writer::ListNode>(&flags," | ", false, false);
    }

    Ref<Node> result = build<Function>().with_children({
      build<Identifier>("PropertyInfo"),
      build_ref<Arguments>(&arguments).with_children({
        build<Argument>().with_child<Literal>("Variant::" + p_variant_type->godot_variant_type()),
        build<Argument>().with_child(Literal::StringLiteral(p_property_name)),
          build<Argument>().with_child<Literal>(p_hint->godot_property_hint()),
        build<Argument>().with_child<Literal>(p_hint->hint_string()),
        property_usage
      })});
    // clang-format on

    if (p_usages.size() == 0) {
      arguments->remove_child(flags);
    } else {
      for (const Ref<GodotPropertyUsageFlagsArgument>& usage : p_usages) {
        flags->add_child(build<Literal>(usage->godot_property_usage_flag()));
      }
    }
    return result;
  }

  Ref<Node> GodotGeneratorUtils::build_property_info(
      const Ref<GodotVariantTypeArgument>& p_variant_type, const String& p_property_name) {
    // clang-format off
    return build<Function>().with_children({
      build<Identifier>("PropertyInfo"),
      build<Arguments>().with_children({
        build<Argument>().with_child<Literal>("Variant::" + p_variant_type->godot_variant_type()),
        build<Argument>().with_child(Literal::StringLiteral(p_property_name)),
      })});
    // clang-format on
  }

  Ref<Node> GodotGeneratorUtils::build_property_info_defaults(
      const Ref<Type>& p_type, const String& p_property_name, DefaultsUsage p_usage) {
    Ref<GodotVariantTypeArgument> variant_type;
    Ref<GodotPropertyHintArgument> property_hint;
    Ref<GodotPropertyUsageFlagsArgument> usage_flags;

    if (!get_defaults_for_type(p_type, variant_type, property_hint, usage_flags, nullptr, p_usage)) {
      return nullptr;
    }

    return build_property_info(variant_type, property_hint, {usage_flags}, p_property_name);
  }

  Ref<GodotPropertyHintArgument> GodotGeneratorUtils::build_property_hint_argument(
      const String& p_value, const String& p_hint_string, bool p_is_string_literal) {
    // clang-format off
    return build<GodotPropertyHintArgument>().with_children({
      build<Identifier>(p_value),
      build<Arguments>().with_children({
          build<Argument>().with_child( p_is_string_literal ? Literal::StringLiteral(p_hint_string) : node_new<Literal>(p_hint_string)),
      })
    });
    // clang-format off
  }


}

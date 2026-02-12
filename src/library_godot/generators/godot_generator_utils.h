/**************************************************************************/
/* godot_generator_utils.h                                                */
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

#pragma once
#include "library/core/core.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/type.h"
#include "library/type_db.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_property_usage_flags.h"
#include "library_godot/attributes/godot_variant_type.h"

namespace GodotObjectCompiler {

  class Node;
  class Function;
  class Type;
  class Context;
  class Class;
  class Body;

  namespace GodotGeneratorUtils {

    Ref<Type> const_ref(const String& p_type_name);

    Ref<Function> bind_method(const String& p_class_name, const String& p_method_name,
        const Vector<String>& p_parameter_names, const Vector<String>& p_default_values = {});

    Ref<Function> bind_static_method(const String& p_class_name, const String& p_method_name,
        const Vector<String>& p_parameter_names, const Vector<String>& p_default_values = {});

    Ref<Body> get_or_create_bind_methods_body(const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body,
        const Ref<Context>& p_generated_sources);

    Ref<Body> get_or_create_get_property_list_body(const Ref<Class>& p_target_class,
        const Ref<Context>& p_generated_body, const Ref<Context>& p_generated_sources);

    Ref<Body> get_or_create_function_names_body(const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body);
    Ref<Body> get_or_create_property_names_body(const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body);
    Ref<Body> get_or_create_signal_names_body(const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body);

    bool check_is_valid_named_argument(const Ref<Node>& p_node, String& p_name);

    bool get_name(const Ref<Node>& p_node, String& p_name);

    bool tree_has_error(const Ref<Node>& p_tree);

    bool class_has_base_class(const Ref<Class>& p_target_class, const String& p_base_class_qualified);

    bool class_is_node_type(const Ref<Class>& p_target_class);

    bool class_is_resource_type(const Ref<Class>& p_target_class);

    bool class_is_ref_counted_type(const Ref<Class>& p_target_class);

    bool class_is_godot_object_type(const Ref<Class>& p_target_class);

    bool class_is_variant_type(Ref<Class> p_target_class);

    template <typename T>
    bool type_is_assumed_template_type(
        const Ref<Type>& p_target_type, AssumeType<T> p_assumed_type, Vector<Ref<Type>>& p_inner_types);

    template <typename T>
    bool type_is_assumed_template_type(
        const Ref<Type>& p_target_type, AssumeType<T> p_assumed_type, Vector<Ref<Type>>& p_inner_types) {
      p_inner_types = {};
      if (p_target_type->type_name_untemplated() != p_assumed_type.qualified_name) {
        return false;
      }

      if (p_target_type->template_argument_count() != p_assumed_type.template_parameter_count) {
        return false;
      }

      Ref<TemplateArguments> template_arguments = p_target_type->template_arguments();
      auto types = template_arguments->find_children<Type>();

      if (types.size() != p_assumed_type.template_parameter_count) {
        return false;
      }

      for (const Ref<Type>& inner_type : types) {
        p_inner_types.push_back(inner_type->clone()->as<Type>());
      }

      return true;
    }

    enum DefaultsUsage {
      DEFAULTS_PROPERTY_BINDING,
      DEFAULTS_SIGNAL_ARGUMENT,
    };

    bool get_defaults_for_type(const Ref<Type>& p_target_type, Ref<GodotVariantTypeArgument>& p_variant_type,
        Ref<GodotPropertyHintArgument>& p_property_hint, Ref<GodotPropertyUsageFlagsArgument>& p_property_usage_flags,
        const Ref<Namespace>& p_from_namespace = nullptr, DefaultsUsage p_defaults_usage = DEFAULTS_PROPERTY_BINDING);

    bool type_is_godot_ref_type(
        const Ref<Type>& p_target_type, Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_godot_typed_array_type(
        const Ref<Type>& p_target_type, Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_godot_typed_dictionary_type(const Ref<Type>& p_target_type, Ref<Type>& p_key_type,
        Ref<Type>& p_value_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_ref_counted_type(const Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_object_type(const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_godot_collection_type(
        const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_node_type(const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_enum_type(
        const Ref<Type>& p_target_type, Ref<Enum>& p_enum_object, const Ref<Namespace>& p_from_namespace = nullptr);

    bool type_is_variant_type(const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace = nullptr);

    bool get_variant_type_from_type(const Ref<Type>& p_target_type, String& p_variant_type);

    Ref<GodotVariantTypeArgument> build_variant_type_argument(const Ref<Type>& p_type);

    Ref<GodotVariantTypeArgument> build_variant_type_argument(const String& p_variant_type);

    Ref<GodotPropertyHintArgument> build_property_hint_argument(
        const String& p_value, const String& p_hint_string = "", bool p_is_string_literal = true);

    Ref<GodotPropertyUsageFlagsArgument> build_property_usage_flags_argument(const String& p_usage);

    Ref<Node> build_property_info(const Ref<GodotVariantTypeArgument>& p_variant_type,
        const Ref<GodotPropertyHintArgument>& p_hint, const Vector<Ref<GodotPropertyUsageFlagsArgument>>& p_usages,
        const String& p_property_name, bool p_no_editor = false);

    Ref<Node> build_property_info(const Ref<GodotVariantTypeArgument>& p_variant_type, const String& p_property_name);

    Ref<Node> build_property_info_defaults(
        const Ref<Type>& p_type, const String& p_property_name, DefaultsUsage p_usage = DEFAULTS_PROPERTY_BINDING);

  };

}

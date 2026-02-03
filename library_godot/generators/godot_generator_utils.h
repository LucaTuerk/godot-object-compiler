
#pragma once
#include "library/core/core.h"
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
    Ref<Type> const_ref(const String& type_name);
    Ref<Function> bind_method(
        const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Ref<Function> bind_static_method(
        const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Ref<Body> get_or_create_bind_methods_body(
        const Ref<Class>& target_class, const Ref<Context>& generated_body, const Ref<Context>& generated_sources);

    bool check_is_valid_named_argument(const Ref<Node>& node, String& name);
    bool get_name(const Ref<Node>& node, String& name);
    bool tree_has_error(const Ref<Node>& tree);
    bool class_has_base_class(const Ref<Class>& target_class, const String& base_class_qualified);
    bool class_is_node_type(const Ref<Class>& target_class);
    bool class_is_resource_type(const Ref<Class>& target_class);
    bool class_is_ref_counted_type(const Ref<Class>& target_class);
    bool class_is_godot_object_type(const Ref<Class>& target_class);
    bool class_is_variant_type(Ref<Class> target_class);

    bool type_is_godot_ref_type(const Ref<Type>& target_type, Ref<Type>& inner_type);
    bool inner_type_is_ref_counted_type(const Ref<Type>& inner_type);
    bool type_is_object_type(const Ref<Type>& target_type);
    bool type_is_node_type(const Ref<Type>& target_type);
    bool type_is_variant_type(const Ref<Type>& target_type);
    bool get_variant_type_from_type(const Ref<Type>& target_type, String& variant_type);

    Ref<GodotVariantTypeArgument> VariantTypeFromType(const Ref<Type>& type);
    Ref<GodotVariantTypeArgument> VariantType(const String& variant_type);
    Ref<GodotPropertyHintArgument> PropertyHint(const String& value, const String& hint_string = "");
    Ref<GodotPropertyUsageFlagsArgument> PropertyUsageFlag(const String& usage);

    Ref<Node> PropertyInfo(const Ref<GodotVariantTypeArgument>& variant_type,
        const Ref<GodotPropertyHintArgument>& hint, const Vector<Ref<GodotPropertyUsageFlagsArgument>>& usages,
        const String& property_name);

    Ref<Node> PropertyInfo(const Ref<GodotVariantTypeArgument>& variant_type, const String& property_name);
    Ref<Node> PropertyInfoDefaultForType(const Ref<Type>& type, const String& property_name);

  };  // namespace GodotGeneratorUtils

}  // namespace GodotObjectCompiler

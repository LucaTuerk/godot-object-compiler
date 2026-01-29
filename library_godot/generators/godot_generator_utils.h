
#pragma once
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class Function;
  class Type;
  class Context;
  class Class;
  class Body;

  namespace GodotGeneratorUtils {
    Ref<Type> const_ref(const String& type_name);
    Ref<Function> bind_method(const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Ref<Function> bind_static_method(const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Ref<Body> get_or_create_bind_methods_body(Ref<Class> target_class, Ref<Context> generated_body, Ref<Context> generated_sources);

    bool class_has_base_class(Ref<Class> target_class, const String& base_class_qualified);
    bool class_is_node_type(Ref<Class> target_class);
    bool class_is_resource_type(Ref<Class> target_class);
    bool class_is_ref_counted_type(Ref<Class> target_class);
    bool class_is_godot_object_type(Ref<Class> target_class);
    bool class_is_variant_type(Ref<Class> target_class);

  };

}  // namespace GodotObjectCompiler


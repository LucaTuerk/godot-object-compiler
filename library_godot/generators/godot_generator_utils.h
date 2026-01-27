
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

  };

}  // namespace GodotObjectCompiler


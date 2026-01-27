
#pragma once
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class Function;
  class Type;
  class Context;
  class Class;
  class Body;

  namespace GodotGeneratorUtils {
    Type* const_ref(const String& type_name);
    Function* bind_method(const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Function* bind_static_method(const String& class_name, const String& method_name, const Vector<String>& parameter_names);
    Body* get_or_create_bind_methods_body(Class* target_class, Context* generated_body, Context* generated_sources);

  };

}  // namespace GodotObjectCompiler


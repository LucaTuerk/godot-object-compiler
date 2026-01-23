#pragma once

#include "../tree/syntax/namespace.h"

namespace GodotObjectCompiler {
  class IGenerator {
   public:
    virtual Context* generate(Context* tree, Node* entry_point) = 0;
  };
}  // namespace GodotObjectCompiler

#define GENERATOR(type)                             \
 public:                                            \
  static String get_type_static() { return #type; } \
                                                    \
 private:

#define GEN_ERROR(message) return node_new<GeneratorError>(get_type_static(), message)
#define GEN_ERROR_COND(condition, message) \
  if ((condition)) {                       \
    GEN_ERROR(message);                    \
  }

#define GEN_ADD_CHILD_ERROR_TO_RESULT(func) \
  {                                         \
    auto error = (func);                    \
    if (error != GeneratorError::OK) {      \
      result->add_child(error);             \
    }                                       \
  }

#define GEN_RETURN_ON_CHILD_ERROR(func) \
  {                                     \
    auto error = (func);                \
    if (error != GeneratorError::OK) {  \
      return error;                     \
    }                                   \
  }

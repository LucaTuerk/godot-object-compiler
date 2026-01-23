
#pragma once
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  class GeneratorError : public Context {
    NODE_TYPE(GeneratorError);

    // No Error is Ok, idk
    static inline GeneratorError* OK = nullptr;

    GeneratorError(const String& generator_name, const String& error_message)
        : generator_name(generator_name), error_message(error_message) {}

    String to_string() const override;
    bool copy_to(Node* other) const override;

    String generator_name;
    String error_message;
  };

}  // namespace GodotObjectCompiler

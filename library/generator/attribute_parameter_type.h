
#pragma once
#include "library/core/core.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  class IAttributeParameterType {
   public:

    virtual ~IAttributeParameterType() = default;

    enum ArgumentType { ARG_STRING };

    enum FeatureFlags {
      FEATURE_NONE = 0,
      FEATURE_FLAG = 1,
    };

    struct Argument {
      ArgumentType type;
      String name;
      bool optional;
    };

    virtual FeatureFlags get_features() { return FEATURE_NONE; }

    virtual String get_type_name() = 0;
    virtual Vector<String> get_value_names() = 0;
    virtual Vector<Argument> get_arguments() = 0;

    virtual Ref<GodotObjectCompiler::Argument> create_argument() { return nullptr; };
  };

}  // namespace GodotObjectCompiler


#pragma once
#include "library/core/assumption.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  class IAttributeParameterType : public IAssumptionValidator<String> {
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

    virtual String get_type() = 0;
    virtual String get_return_type() = 0;
    virtual Vector<String> get_value_names() = 0;
    virtual Vector<Argument> get_arguments() = 0;

    virtual Ref<GodotObjectCompiler::Argument> create_argument() { return nullptr; };

    AssumptionState validate_assumption(Assumption<std::string>& assumption) override;
  };

  inline AssumptionState IAttributeParameterType::validate_assumption(Assumption<String>& assumption) {
    if (Vector<String> values = get_value_names();
        std::find(values.begin(), values.end(), assumption()) == values.end()) {
      return STATE_INVALID;
    }

    return STATE_VALID;
  }

#define PARAM_TYPE(type)                            \
 public:                                            \
                                                    \
  static Ref<type> instance() {                     \
    static Ref<type> _instance = make_ref<type>();  \
    return _instance;                               \
  }                                                 \
  String get_type() override { return #type; }      \
  static String get_type_static() { return #type; } \
                                                    \
 private:

}  // namespace GodotObjectCompiler

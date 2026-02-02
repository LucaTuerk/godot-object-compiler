
#pragma once
#include "core.h"
#include "helpers.h"

namespace GodotObjectCompiler {

  enum AssumptionState { STATE_INDETERMINATE, STATE_VALID, STATE_INVALID };

  template <typename T>
  class Assumption;

  template <typename T>
  class IAssumptionValidator {
   public:

    virtual ~IAssumptionValidator() = default;

    virtual AssumptionState validate_assumption(Assumption<T>& assumption) = 0;
  };

  class IAssumptionSet {
   public:

    virtual ~IAssumptionSet() = default;

    virtual void validate_assumptions() = 0;
  };

  template <typename T>
  class Assumption {
   public:

    using Validator = std::function<AssumptionState(Assumption& assumption)>;

    Assumption(const T& value, const String& unvalidated_message);
    ~Assumption();

    const T& operator()() const;
    AssumptionState validate(Validator validator);
    AssumptionState validate(IAssumptionValidator<T>* validator);

    [[nodiscard]] bool is_ok() const;
    [[nodiscard]] bool is_valid() const;
    [[nodiscard]] bool is_invalid() const;

   public:

    AssumptionState state = STATE_INDETERMINATE;
    T value;
    mutable Size value_access_count = 0;
    bool was_validated;
    String message;
  };

  template <typename T>
  Assumption<T>::Assumption(const T& value, const String& unvalidated_message) {
    this->value = value;
    message = unvalidated_message;
  }

  template <typename T>
  Assumption<T>::~Assumption() {
#ifdef ASSUMPTIONS_ENABLED
    if (state == STATE_INDETERMINATE) {
      print_err("Assumption was left in a indeterminate state: " + message);
    }

    if (state == STATE_INVALID) {
      if (value_access_count == 0) {
        print_err(format("Assumption did not hold but was never accessed: %s", message.c_str()));
      } else {
        print_err(format("Assumption did not hold and was accessed %d times: %s", value_access_count, message.c_str()));
      }
    }
#endif
  }

  template <typename T>
  const T& Assumption<T>::operator()() const {
    if (state == STATE_INVALID) {
      PANIC("Trying to access an invalid Assumption: %s", message.c_str());
    }
    value_access_count++;
    return value;
  }

  template <typename T>
  AssumptionState Assumption<T>::validate(Validator validator) {
    if (!state == STATE_INDETERMINATE) {
      return state;
    }

    state = validator(value);
    return state;
  }

  template <typename T>
  AssumptionState Assumption<T>::validate(IAssumptionValidator<T>* validator) {
    if (!state == STATE_INDETERMINATE) {
      return state;
    }

    state = validator->validate_assumption(*this);
    return state;
  }

  template <typename T>
  bool Assumption<T>::is_ok() const {
    return state == STATE_VALID || state == STATE_INDETERMINATE;
  }

  template <typename T>
  bool Assumption<T>::is_valid() const {
    return state == STATE_VALID;
  }

  template <typename T>
  bool Assumption<T>::is_invalid() const {
    return state == STATE_INVALID;
  }

}  // namespace GodotObjectCompiler

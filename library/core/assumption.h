/**************************************************************************/
/* assumption.h                                                           */
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
#include "core.h"

namespace GodotObjectCompiler {

  enum AssumptionState { STATE_INDETERMINATE, STATE_VALID, STATE_INVALID };

  template <typename T>
  class Assumption;

  template <typename T>
  class IAssumptionValidator {
   public:

    virtual ~IAssumptionValidator() = default;

    virtual AssumptionState validate_assumption(Assumption<T>& p_assumption) = 0;
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
    AssumptionState validate(Validator p_validator);
    AssumptionState validate(IAssumptionValidator<T>* p_validator);

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
  Assumption<T>::Assumption(const T& value, const String& unvalidated_message) : was_validated(false) {
    this->value = value;
    message = unvalidated_message;
  }

  template <typename T>
  Assumption<T>::~Assumption() {
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
  AssumptionState Assumption<T>::validate(Validator p_validator) {
    if (!state == STATE_INDETERMINATE) {
      return state;
    }

    state = p_validator(value);
    return state;
  }

  template <typename T>
  AssumptionState Assumption<T>::validate(IAssumptionValidator<T>* p_validator) {
    if (!state == STATE_INDETERMINATE) {
      return state;
    }

    state = p_validator->validate_assumption(*this);
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

}

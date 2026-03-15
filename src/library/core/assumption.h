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

enum AssumptionState { STATE_INDETERMINATE,
	STATE_VALID,
	STATE_INVALID };

template <typename T>
class Assumption;

template <typename T>
class IAssumptionValidator {
public:
	virtual ~IAssumptionValidator() = default;

	virtual AssumptionState validate_assumption(Assumption<T> &p_assumption) = 0;
};

class IAssumptionSet {
public:
	virtual ~IAssumptionSet() = default;

	virtual void validate_assumptions() = 0;
};

template <typename T>
class Assumption {
public:
	using Validator = std::function<AssumptionState(Assumption &assumption)>;

	Assumption(
			const T &value, const String &unvalidated_message,
			IAssumptionValidator<T> *p_validator = nullptr);
	~Assumption();

	const T &operator()() const;
	const T &unwrap();

	AssumptionState validate(Validator p_validator);
	AssumptionState validate(IAssumptionValidator<T> *p_validator);

	[[nodiscard]] bool is_ok() const;
	[[nodiscard]] bool is_valid() const;
	[[nodiscard]] bool is_invalid() const;

private:
	bool was_validated;
	AssumptionState state = STATE_INDETERMINATE;
	mutable Size value_access_count = 0;
	String message;
	T value;
	IAssumptionValidator<T> *validator = nullptr;

	friend class UNSAFE_VALUE_EXTRACTOR;
};

class UNSAFE_VALUE_EXTRACTOR {
public:
	template <typename T>
	static T &GET_VERY_UNSAFELY(Assumption<T> &p_assumption);
};

template <typename T>
Assumption<T>::Assumption(
		const T &value, const String &unvalidated_message, IAssumptionValidator<T> *p_validator) : was_validated(false),
																								   validator(p_validator) {
	this->value = value;
	message = unvalidated_message;
}

template <typename T>
Assumption<T>::~Assumption() {
	if (value_access_count == 0) {
		return;
	}

	if (state == STATE_INDETERMINATE) {
		fmt_print_err("Assumption was left in a indeterminate state: " + message);
	}

	if (state == STATE_INVALID) {
		fmt_print_err(format(
				"Assumption did not hold and was accessed %d times: %s", value_access_count,
				message.c_str()));
	}
}

template <typename T>
const T &Assumption<T>::operator()() const {
	if (state != STATE_VALID) {
		PANIC("Trying to access an unvalidated Assumption: %s", message.c_str());
	}
	value_access_count++;
	return value;
}

template <typename T>
const T &Assumption<T>::unwrap() {
	if (state == STATE_INDETERMINATE && validator != nullptr) {
		state = validator->validate_assumption(*this);
	}

	return operator()();
}

template <typename T>
AssumptionState Assumption<T>::validate(Validator p_validator) {
	state = p_validator(value);
	return state;
}

template <typename T>
AssumptionState Assumption<T>::validate(IAssumptionValidator<T> *p_validator) {
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

template <typename T>
T &UNSAFE_VALUE_EXTRACTOR::GET_VERY_UNSAFELY(Assumption<T> &p_assumption) {
	return p_assumption.value;
}
} // namespace GodotObjectCompiler

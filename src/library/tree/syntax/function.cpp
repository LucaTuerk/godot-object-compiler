/**************************************************************************/
/* function.cpp                                                           */
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
#include "function.h"

#include "access_specifier.h"
#include "class.h"
#include "modifiers.h"
#include "struct.h"
#include "type.h"

namespace GodotObjectCompiler {

  Ref<Type> Function::_type_lazy_get() { return find_child<Type>(); }

  Ref<Parameters> Function::_parameters_lazy_get() { return find_child<Parameters>(); }

  Ref<Arguments> Function::_arguments_lazy_get() { return find_child<Arguments>(); }

  bool Function::copy_to(const Ref<Node>& p_other) const {
    COPY_GUARD(Function, Member);
    // COPY_LAZY(type);
    // COPY_LAZY(parameters);
    // COPY_LAZY(is_virtual);
    // COPY_LAZY(is_override);
    // COPY_LAZY(is_static);
    // COPY_LAZY(is_const);
    return true;
  }

  Ref<Type> Parameter::_type_lazy_get() { return find_child<Type>(); }

  Ref<Literal> Parameter::_default_value_lazy_get() { return find_child<Literal>(); }

  Vector<Ref<Literal>> Function::_default_values_lazy_get() {
    Vector<Ref<Literal>> default_value_literals;
    if (!parameters()) {
      return {};
    }

    for (const Ref<Parameter>& parameter : parameters()->find_children<Parameter>()) {
      if (parameter->default_value()) {
        default_value_literals.push_back(parameter->default_value());
      }
    }

    return default_value_literals;
  }

}
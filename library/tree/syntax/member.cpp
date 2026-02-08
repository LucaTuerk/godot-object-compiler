/**************************************************************************/
/* member.cpp                                                             */
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

#include "member.h"

#include "access_specifier.h"
#include "class.h"
#include "modifiers.h"
#include "struct.h"

namespace GodotObjectCompiler {

  bool Member::_is_virtual_lazy_get() { return find_child<Virtual>() != nullptr; }

  bool Member::_is_override_lazy_get() { return find_child<Override>() != nullptr; }

  bool Member::_is_static_lazy_get() { return find_child<Static>() != nullptr; }

  bool Member::_is_const_lazy_get() { return find_child<Const>() != nullptr; }

  Ref<AccessSpecifier::Type> Member::_access_specifier_type_lazy_get() {
    if (!get_parent()) {
      return nullptr;
    }

    const bool is_in_class = get_parent()->is<Class>();
    const bool is_in_struct = get_parent()->is<Struct>();

    if (!is_in_class && !is_in_struct) {
      return nullptr;
    }

    const Ref<AccessSpecifier> specifier = find_previous_sibling<AccessSpecifier>();
    if (!specifier) {
      if (is_in_class) {
        return make_ref<AccessSpecifier::Type>(AccessSpecifier::PUBLIC);
      } else {
        return make_ref<AccessSpecifier::Type>(AccessSpecifier::PRIVATE);
      }
    }

    return make_ref<AccessSpecifier::Type>(specifier->type);
  }

  bool Member::_is_public_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PUBLIC;
  }

  bool Member::_is_protected_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PROTECTED;
  }

  bool Member::_is_private_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PRIVATE;
  }

}
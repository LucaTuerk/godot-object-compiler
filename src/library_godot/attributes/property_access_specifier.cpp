/**************************************************************************/
/* property_access_specifier.cpp                                          */
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

#include "property_access_specifier.h"

#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler {

String PropertyGetAccessSpecifierParameterType::get_return_type() {
  return "GOC_PropertyGetAccessSpecifier";
}

Vector<String> PropertyGetAccessSpecifierParameterType::get_value_names() {
  return {PropertyGetAccessSpecifierArgument::PrivateGet,
          PropertyGetAccessSpecifierArgument::ProtectedGet,
          PropertyGetAccessSpecifierArgument::PublicGet};
}

Vector<IAttributeParameterType::Argument>
PropertyGetAccessSpecifierParameterType::get_arguments() {
  return {};
}

String PropertySetAccessSpecifierParameterType::get_return_type() {
  return "GOC_PropertySetAccessSpecifier";
}

Vector<String> PropertySetAccessSpecifierParameterType::get_value_names() {
  return {PropertySetAccessSpecifierArgument::PrivateSet,
          PropertySetAccessSpecifierArgument::ProtectedSet,
          PropertySetAccessSpecifierArgument::PublicSet};
}

Vector<IAttributeParameterType::Argument>
PropertySetAccessSpecifierParameterType::get_arguments() {
  return {};
}

bool PropertyGetAccessSpecifierArgument::get_specifier(
    AccessSpecifier::Type& r_specifier) const {
  const Ref<Identifier> identifier = find_child<Identifier>();
  if (!identifier) {
    ERR("Failed to find identifier.");
    return false;
  }

  if (identifier->name == PrivateGet) {
    r_specifier = AccessSpecifier::PRIVATE;
    return true;
  } else if (identifier->name == ProtectedGet) {
    r_specifier = AccessSpecifier::PROTECTED;
    return true;
  } else if (identifier->name == PublicGet) {
    r_specifier = AccessSpecifier::PUBLIC;
    return true;
  }

  return false;
}

bool PropertyGetAccessSpecifierArgument::get_specifier_cpp_name(
    String& r_specifier_name) const {
  r_specifier_name = "";
  const Ref<Identifier> identifier = find_child<Identifier>();
  if (!identifier) {
    ERR("Failed to find identifier.");
    return false;
  }

  if (identifier->name == PrivateGet) {
    r_specifier_name = "private";
    return true;
  } else if (identifier->name == ProtectedGet) {
    r_specifier_name = "protected";
    return true;
  } else if (identifier->name == PublicGet) {
    r_specifier_name = "public";
    return true;
  }

  ERR("Invalid identifier name %s.", identifier->name.c_str());
  return false;
}

bool PropertySetAccessSpecifierArgument::get_specifier(
    AccessSpecifier::Type& r_specifier) const {
  const Ref<Identifier> identifier = find_child<Identifier>();
  if (!identifier) {
    ERR("Failed to find identifier.");
    return false;
  }

  if (identifier->name == PrivateSet) {
    r_specifier = AccessSpecifier::PRIVATE;
    return true;
  } else if (identifier->name == ProtectedSet) {
    r_specifier = AccessSpecifier::PROTECTED;
    return true;
  } else if (identifier->name == PublicSet) {
    r_specifier = AccessSpecifier::PUBLIC;
    return true;
  }

  return false;
}

bool PropertySetAccessSpecifierArgument::get_specifier_cpp_name(
    String& r_specifier_name) const {
  r_specifier_name = "";
  const Ref<Identifier> identifier = find_child<Identifier>();
  if (!identifier) {
    ERR("Failed to find identifier.");
    return false;
  }

  if (identifier->name == PrivateSet) {
    r_specifier_name = "private";
    return true;
  } else if (identifier->name == ProtectedSet) {
    r_specifier_name = "protected";
    return true;
  } else if (identifier->name == PublicSet) {
    r_specifier_name = "public";
    return true;
  }

  ERR("Invalid identifier name %s.", identifier->name.c_str());
  return false;
}

}  // namespace GodotObjectCompiler
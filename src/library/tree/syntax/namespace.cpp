/**************************************************************************/
/* namespace.cpp                                                          */
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

#include "namespace.h"

#include "class.h"
#include "enum.h"
#include "field.h"
#include "function.h"

namespace GodotObjectCompiler
{

Vector<Ref<Namespace>> Namespace::_namespaces_lazy_get() const
{
  Vector<Ref<Namespace>> reversed;
  Ref<Context> current = get_parent();

  while (current) {
    if (current->is<Namespace>()) {
      reversed.push_back(current->as<Namespace>());
    }
    current = current->get_parent();
  }

  Vector<Ref<Namespace>> result;
  for (auto itr = reversed.rbegin(); itr != reversed.rend(); ++itr) {
    result.push_back(*itr);
  }

  return result;
}

Vector<Ref<Namespace>> Namespace::_child_namespaces_lazy_get() const
{
  Vector<Ref<Namespace>> _namespaces;

  for (const Ref<Node>& child : _children) {
    if (child->is<Namespace>()) {
      _namespaces.push_back(child->as<Namespace>());
    }
  }

  return _namespaces;
}

Vector<Ref<Field>> Namespace::_fields_lazy_get() const
{
  return body() ? body()->find_children<Field>() : Vector<Ref<Field>>();
}

Vector<Ref<Function>> Namespace::_functions_lazy_get() const
{
  return body() ? body()->find_children<Function>() : Vector<Ref<Function>>();
}

Vector<Ref<Class>> Namespace::_classes_lazy_get() const
{
  return body() ? body()->find_children<Class>() : Vector<Ref<Class>>();
}

Vector<Ref<Class>> Namespace::_classes_recursive_lazy_get() const
{
  return body() ? body()->find_children<Class>(true) : Vector<Ref<Class>>();
}

Vector<Ref<Enum>> Namespace::_enums_lazy_get() const
{
  return body() ? body()->find_children<Enum>() : Vector<Ref<Enum>>();
}

Vector<Ref<Enum>> Namespace::_enums_recursive_lazy_get() const
{
  return body() ? body()->find_children<Enum>(true) : Vector<Ref<Enum>>();
}

} // namespace GodotObjectCompiler
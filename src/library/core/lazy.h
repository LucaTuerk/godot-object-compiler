/**************************************************************************/
/* lazy.h                                                                 */
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

#define LAZY(classname, type, name)                                                   \
 private:                                                                             \
                                                                                      \
  type _##name##_lazy_get() const;                                                    \
  mutable Lazy<type, classname> _##name##_lazy{this, &classname::_##name##_lazy_get}; \
                                                                                      \
 public:                                                                              \
                                                                                      \
  type const& name() const { return _##name##_lazy.get(); }                           \
                                                                                      \
 private:

#define LAZY_MUT(classname, type, name)                                               \
 private:                                                                             \
                                                                                      \
  type _##name##_lazy_get();                                                          \
  mutable Lazy<type, classname> _##name##_lazy{this, &classname::_##name##_lazy_get}; \
                                                                                      \
 public:                                                                              \
                                                                                      \
  type const& name() const { return _##name##_lazy.get(); }                           \
                                                                                      \
 private:

namespace GodotObjectCompiler {

  template <typename T, typename C>

  class Lazy {
   public:

    using Getter = T (C::*)();
    using ConstGetter = T (C::*)() const;

    Lazy(C* obj, Getter getter) : _obj(obj), _getter(getter), _is_const(false) {}

    Lazy(C* obj, ConstGetter getter) : _obj(obj), _const_getter(getter), _is_const(true) {}

    void operator=(T const& data) {
      _data = data;
      _has_data = true;
    }

    void poke() const;

    T const& get() const;

   private:

    C* _obj;
    Getter _getter;
    ConstGetter _const_getter;
    mutable bool _is_const = false;
    mutable bool _has_data = false;
    mutable T _data;
  };

  template <typename T, typename C>
  void Lazy<T, C>::poke() const {
    if (!_has_data) {
      if (_is_const) {
        _data = (_obj->*_const_getter)();
      } else {
        _data = (_obj->*_getter)();
      }
      _has_data = true;
    }
  }

  template <typename T, typename C>
  const T& Lazy<T, C>::get() const {
    poke();
    return _data;
  }

}

#pragma once

#define LAZY(classname, type, name)                                                   \
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

    Lazy(C* obj, Getter getter) : _obj(obj), _getter(getter) {}

    void operator=(T const& data) {
      _data = data;
      _has_data = true;
    }

    T const& get() const;

   private:

    C* _obj;
    Getter _getter;
    mutable bool _has_data = false;
    mutable T _data;
  };

  template <typename T, typename C>
  const T& Lazy<T, C>::get() const {
    if (!_has_data) {
      _data = (_obj->*_getter)();
      _has_data = true;
    }
    return _data;
  }

}  // namespace GodotObjectCompiler

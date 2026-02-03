
#include "namespace.h"

#include "class.h"
#include "enum.h"
#include "field.h"
#include "function.h"

namespace GodotObjectCompiler {

  Vector<Ref<Namespace>> Namespace::_namespaces_lazy_get() {
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

  Vector<Ref<Namespace>> Namespace::_child_namespaces_lazy_get() {
    Vector<Ref<Namespace>> _namespaces;

    for (Ref<Node> child : *this) {
      if (child->is<Namespace>()) {
        _namespaces.push_back(child->as<Namespace>());
      }
    }

    return _namespaces;
  }

  Vector<Ref<Field>> Namespace::_fields_lazy_get() {
    return body() ? body()->find_children<Field>() : Vector<Ref<Field>>();
  }

  Vector<Ref<Function>> Namespace::_functions_lazy_get() {
    return body() ? body()->find_children<Function>() : Vector<Ref<Function>>();
  }

  Vector<Ref<Class>> Namespace::_classes_lazy_get() {
    return body() ? body()->find_children<Class>() : Vector<Ref<Class>>();
  }

  Vector<Ref<Class>> Namespace::_classes_recursive_lazy_get() {
    return body() ? body()->find_children<Class>(true) : Vector<Ref<Class>>();
  }

  Vector<Ref<Enum>> Namespace::_enums_lazy_get() {
    return body() ? body()->find_children<Enum>() : Vector<Ref<Enum>>();
  }

  Vector<Ref<Enum>> Namespace::_enums_recursive_lazy_get() {
    return body() ? body()->find_children<Enum>(true) : Vector<Ref<Enum>>();
  }

}  // namespace GodotObjectCompiler
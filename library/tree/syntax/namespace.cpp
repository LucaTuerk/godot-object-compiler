
#include "namespace.h"

#include "class.h"
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
    Vector<Ref<Field>> fields;
    Ref<Body> body = find_child<Body>();
    const List<Ref<Node>>& children = body ? body->get_children() : get_children();

    for (Ref<Node> child : children) {
      if (child->is<Field>()) {
        fields.push_back(child->as<Field>());
      }
    }

    return fields;
  }

  Vector<Ref<Function>> Namespace::_functions_lazy_get() {
    Vector<Ref<Function>> functions;
    Ref<Body> body = find_child<Body>();
    const List<Ref<Node>>& children = body ? body->get_children() : get_children();

    for (Ref<Node> child : children) {
      if (child->is<Function>()) {
        functions.push_back(child->as<Function>());
      }
    }

    for (Ref<Namespace> _namespace : namespaces()) {
      for (Ref<Function> _class : _namespace->functions()) {
        functions.push_back(_class);
      }
    }

    return functions;
  }

  Vector<Ref<Class>> Namespace::_classes_lazy_get() {
    Vector<Ref<Class>> classes;
    Ref<Body> body = find_child<Body>();
    const List<Ref<Node>>& children = body ? body->get_children() : get_children();

    for (Ref<Node> child : children) {
      if (child->is<Class>()) {
        classes.push_back(child->as<Class>());
      }
    }

    return classes;
  }

  Vector<Ref<Class>> Namespace::_classes_recursive_lazy_get() {
    Vector<Ref<Class>> _classes = classes();

    for (Ref<Namespace> _child_namespace : child_namespaces()) {
      for (Ref<Class> _class : _child_namespace->classes_recursive()) {
        _classes.push_back(_class);
      }
    }

    return _classes;
  }

}  // namespace GodotObjectCompiler
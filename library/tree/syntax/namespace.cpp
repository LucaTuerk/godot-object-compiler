
#include "namespace.h"

#include "class.h"
#include "field.h"
#include "function.h"

namespace GodotObjectCompiler {

  Vector<Namespace*> Namespace::_namespaces_lazy_get() {
    Vector<Namespace*> reversed;
    Context* current = get_parent();

    while (current) {
      if (current->is<Namespace>()) {
        reversed.push_back(current->as<Namespace>());
      }
      current = current->get_parent();
    }

    Vector<Namespace*> result;
    for (auto itr = reversed.rbegin(); itr != reversed.rend(); ++itr) {
      result.push_back(*itr);
    }

    return result;
  }

  Vector<Namespace*> Namespace::_child_namespaces_lazy_get() {
    Vector<Namespace*> _namespaces;

    for (Node* child : *this) {
      if (child->is<Namespace>()) {
        _namespaces.push_back(child->as<Namespace>());
      }
    }

    return _namespaces;
  }

  Vector<Field*> Namespace::_fields_lazy_get() {
    Vector<Field*> fields;
    Body* body = find_child<Body>();
    const List<Node*>& children = body ? body->get_children() : get_children();

    for (Node* child : children) {
      if (child->is<Field>()) {
        fields.push_back(child->as<Field>());
      }
    }

    return fields;
  }

  Vector<Function*> Namespace::_functions_lazy_get() {
    Vector<Function*> functions;
    Body* body = find_child<Body>();
    const List<Node*>& children = body ? body->get_children() : get_children();

    for (Node* child : children) {
      if (child->is<Function>()) {
        functions.push_back(child->as<Function>());
      }
    }

    for (Namespace* _namespace : namespaces()) {
      for (Function* _class : _namespace->functions()) {
        functions.push_back(_class);
      }
    }

    return functions;
  }

  Vector<Class*> Namespace::_classes_lazy_get() {
    Vector<Class*> classes;
    Body* body = find_child<Body>();
    const List<Node*>& children = body ? body->get_children() : get_children();

    for (Node* child : children) {
      if (child->is<Class>()) {
        classes.push_back(child->as<Class>());
      }
    }

    return classes;
  }

  Vector<Class*> Namespace::_classes_recursive_lazy_get() {
    Vector<Class*> _classes = classes();

    for (Namespace* _child_namespace : child_namespaces()) {
      for (Class* _class : _child_namespace->classes_recursive()) {
        _classes.push_back(_class);
      }
    }

    return _classes;
  }

}  // namespace GodotObjectCompiler
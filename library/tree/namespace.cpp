
#include "namespace.h"

#include "class.h"
#include "field.h"
#include "function.h"
#include "identifier.h"

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

  Vector<Field*> Namespace::_fields_lazy_get() {
    Vector<Field*> fields;
    Body* body = find_child<Body>();
    const Vector<Node*>& children =
        body ? body->get_children() : get_children();

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
    const Vector<Node*>& children =
        body ? body->get_children() : get_children();

    for (Node* child : children) {
      if (child->is<Function>()) {
        functions.push_back(child->as<Function>());
      }
    }

    return functions;
  }

}  // namespace GodotObjectCompiler
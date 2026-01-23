
#include "class.h"

#include "attribute.h"
#include "attributes_godot.h"
#include "identifier.h"

namespace GodotObjectCompiler {

  Vector<Class*> Class::_base_classes_lazy_get() {
    Vector<Identifier*> identifiers;

    bool first = true;
    for (Node* child : get_children()) {
      Identifier* child_identifier = child->as<Identifier>();
      if (child_identifier) {
        if (first) {
          first = false;
        } else {
          identifiers.push_back(child_identifier);
        }
      }
    }

    if (identifiers.empty()) {
      return {};
    }

    Vector<Class*> classes;
    Context* root = get_root()->as<Context>();
    if (root) {
      for (Identifier* base_id : identifiers) {
        Class* base = root->find_descendant<Class>(BFS, [base_id](Class* some_class) {
          auto qualified_name = some_class->qualified_name();
          return qualified_name == base_id->name;
        });

        if (base) {
          classes.push_back(base);
          Vector<Class*> base_base_classes = base->base_classes();
          for (Class* base_base_class : base_base_classes) {
            classes.push_back(base_base_class);
          }
        }
      }
    }
    return classes;
  }

  Body* Class::_class_body_lazy_get() { return find_child<Body>(); }

  Vector<Attribute*> Class::_attributes_lazy_get() {
    Vector<Attribute*> attributes;
    if (!class_body()) {
      return attributes;
    }

    for (Node* child : class_body()->get_children()) {
      Attribute* child_attribute = child->as<Attribute>();
      if (child_attribute) {
        attributes.push_back(child_attribute->as<Attribute>());
      }
    }

    return attributes;
  }

  bool Class::_has_generated_class_attribute_lazy_get() {
    return find_descendant<GodotGeneratedBodyAttribute>() != nullptr;
  }

  bool Class::copy_to(Node* other) const {
    COPY_GUARD(Class, Namespace);
    // COPY_LAZY(base_classes);
    return true;
  }

}  // namespace GodotObjectCompiler
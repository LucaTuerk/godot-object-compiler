
#include "class.h"

#include "../../../library_godot/attributes/godot_attributes.h"
#include "attribute.h"
#include "identifier.h"

namespace GodotObjectCompiler {

  Vector<String> Class::_direct_bases_names_lazy_get() {
    Vector<String> direct_bases_names;

    bool first = true;
    for (Ref<Node> child : *this) {
      Ref<Identifier> child_identifier = child->as<Identifier>();
      if (child_identifier) {
        if (first) {
          first = false;
        } else {
          direct_bases_names.push_back(child_identifier->name);
        }
      }
    }

    return direct_bases_names;
  }

  Vector<Ref<Class>> Class::_base_classes_lazy_get() {
    Vector<Ref<Identifier>> identifiers;

    bool first = true;
    for (Ref<Node> child : get_children()) {
      Ref<Identifier> child_identifier = child->as<Identifier>();
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

    Vector<Ref<Class>> classes;
    Ref<Context> root = get_root()->as<Context>();
    if (root) {
      for (Ref<Identifier> base_id : identifiers) {
        Ref<Class> base = root->find_descendant<Class>(BFS, [base_id](Ref<Class> some_class) {
          auto qualified_name = some_class->qualified_name();
          return qualified_name == base_id->name;
        });

        if (base) {
          classes.push_back(base);
          Vector<Ref<Class>> base_base_classes = base->base_classes();
          for (Ref<Class> base_base_class : base_base_classes) {
            classes.push_back(base_base_class);
          }
        }
      }
    }
    return classes;
  }

  Vector<Ref<Attribute>> Class::_attributes_lazy_get() {
    Vector<Ref<Attribute>> attributes;
    if (!body()) {
      return attributes;
    }

    for (Ref<Node> child : body()->get_children()) {
      Ref<Attribute> child_attribute = child->as<Attribute>();
      if (child_attribute) {
        attributes.push_back(child_attribute->as<Attribute>());
      }
    }

    return attributes;
  }

  bool Class::_has_generated_class_attribute_lazy_get() {
    return find_descendant<GodotGeneratedBodyAttribute>() != nullptr;
  }

  bool Class::copy_to(Ref<Node> other) const {
    COPY_GUARD(Class, Namespace);
    // COPY_LAZY(base_classes);
    return true;
  }

}  // namespace GodotObjectCompiler
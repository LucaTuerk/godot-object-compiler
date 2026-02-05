
#include "class.h"

#include "attribute.h"
#include "identifier.h"
#include "library/type_db.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  Vector<String> Class::_direct_bases_names_lazy_get() {
    Vector<String> result;

    const Ref<BaseClasses> base_classes = find_child<BaseClasses>();
    if (!base_classes) {
      return {};
    }

    for (const Ref<Type>& type : base_classes->find_children<Type>()) {
      result.push_back(type->name());
    }

    return result;
  }

  Vector<Ref<Class>> Class::_base_classes_lazy_get() {
    Vector<Ref<Identifier>> identifiers;

    Ref<BaseClasses> bases = find_child<BaseClasses>();
    if (!bases) {
      return {};
    }

    Vector<Ref<Class>> result;
    for (const Ref<Type>& type : bases->find_children<Type>()) {
      Ref<Class> base_class = TypeDB::instance()->get_type_data<Class>(type);
      if (base_class) {
        result.push_back(base_class);
      }
    }

    return result;
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

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

  bool Class::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(Class, Namespace);
    // COPY_LAZY(base_classes);
    return true;
  }

  Vector<Ref<Function>> Class::_member_functions_lazy_get() {
    if (!body()) {
      return {};
    }

    return body()->find_children<Function>();
  }

  Vector<Ref<Function>> Class::_public_member_functions_lazy_get() {
    Vector<Ref<Function>> result;
    for (const Ref<Function>& member_function : member_functions()) {
      if (member_function->is_public_member()) {
        result.push_back(member_function);
      }
    }
    return result;
  }

  Vector<Ref<Function>> Class::_protected_member_functions_lazy_get() {
    Vector<Ref<Function>> result;
    for (const Ref<Function>& member_function : member_functions()) {
      if (member_function->is_protected_member()) {
        result.push_back(member_function);
      }
    }
    return result;
  }

  Vector<Ref<Function>> Class::_private_member_functions_lazy_get() {
    Vector<Ref<Function>> result;
    for (const Ref<Function>& member_function : member_functions()) {
      if (member_function->is_private_member()) {
        result.push_back(member_function);
      }
    }
    return result;
  }

  Vector<Ref<Field>> Class::_member_fields_lazy_get() {
    if (!body()) {
      return {};
    }

    return body()->find_children<Field>();
  }

  Vector<Ref<Field>> Class::_public_member_fields_lazy_get() {
    Vector<Ref<Field>> result;
    for (const Ref<Field>& member_field : member_fields()) {
      if (member_field->is_public_member()) {
        result.push_back(member_field);
      }
    }
    return result;
  }

  Vector<Ref<Field>> Class::_protected_member_fields_lazy_get() {
    Vector<Ref<Field>> result;
    for (const Ref<Field>& member_field : member_fields()) {
      if (member_field->is_protected_member()) {
        result.push_back(member_field);
      }
    }
    return result;
  }

  Vector<Ref<Field>> Class::_private_member_fields_lazy_get() {
    Vector<Ref<Field>> result;
    for (const Ref<Field>& member_field : member_fields()) {
      if (member_field->is_private_member()) {
        result.push_back(member_field);
      }
    }
    return result;
  }

}  // namespace GodotObjectCompiler
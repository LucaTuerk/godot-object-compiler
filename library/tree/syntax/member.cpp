
#include "member.h"

#include "access_specifier.h"
#include "class.h"
#include "modifiers.h"
#include "struct.h"

namespace GodotObjectCompiler {

  bool Member::_is_virtual_lazy_get() { return find_child<Virtual>() != nullptr; }

  bool Member::_is_override_lazy_get() { return find_child<Override>() != nullptr; }

  bool Member::_is_static_lazy_get() { return find_child<Static>() != nullptr; }

  bool Member::_is_const_lazy_get() { return find_child<Const>() != nullptr; }

  Ref<AccessSpecifier::Type> Member::_access_specifier_type_lazy_get() {
    if (!get_parent()) {
      return nullptr;
    }

    const bool is_in_class = get_parent()->is<Class>();
    const bool is_in_struct = get_parent()->is<Struct>();

    if (!is_in_class && !is_in_struct) {
      return nullptr;
    }

    const Ref<AccessSpecifier> specifier = find_previous_sibling<AccessSpecifier>();
    if (!specifier) {
      if (is_in_class) {
        return make_ref<AccessSpecifier::Type>(AccessSpecifier::PUBLIC);
      } else {
        return make_ref<AccessSpecifier::Type>(AccessSpecifier::PRIVATE);
      }
    }

    return make_ref<AccessSpecifier::Type>(specifier->type);
  }

  bool Member::_is_public_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PUBLIC;
  }

  bool Member::_is_protected_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PROTECTED;
  }

  bool Member::_is_private_member_lazy_get() {
    return access_specifier_type() && *access_specifier_type() == AccessSpecifier::PRIVATE;
  }

}  // namespace GodotObjectCompiler

#pragma once
#include "access_specifier.h"
#include "context.h"

namespace GodotObjectCompiler {

  class Member : public NamedContext {
    NODE_TYPE(Member)
    /* info */
    LAZY(Member, bool, is_virtual);
    LAZY(Member, bool, is_override);
    LAZY(Member, bool, is_static);
    LAZY(Member, bool, is_const);
    LAZY(Member, Ref<AccessSpecifier::Type>, access_specifier_type);
    LAZY(Member, bool, is_public_member);
    LAZY(Member, bool, is_protected_member);
    LAZY(Member, bool, is_private_member);
  };

}  // namespace GodotObjectCompiler

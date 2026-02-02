#include "type.h"

#include "identifier.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "modifiers.h"

namespace GodotObjectCompiler {

  String Type::_type_name_lazy_get() {
    StreamWriter writer;

    for (Ref<Node> child : get_children()) {
      if (Ref<Identifier> id = child->as<Identifier>()) {
        writer.write(id->name);
      } else if (Ref<Reference> ref = child->as<Reference>()) {
        writer.write("&");
      } else if (Ref<Pointer> ptr = child->as<Pointer>()) {
        writer.write("*");
      } else if (Ref<Const> c = child->as<Const>()) {
        writer.write(" const ");
      } else if (Ref<Static> s = child->as<Static>()) {
        writer.write(" static ");
      } else if (Ref<Volatile> v = child->as<Volatile>()) {
        writer.write(" volatile ");
      } else if (Ref<Unsigned> u = child->as<Unsigned>()) {
        writer.write(" unsigned ");
      } else if (Ref<Signed> s = child->as<Signed>()) {
        writer.write(" signed ");
      } else if (Ref<Short> s = child->as<Short>()) {
        writer.write(" short ");
      } else if (Ref<Long> s = child->as<Long>()) {
        writer.write(" long ");
      }
    }

    return string_shrink_inner_space(string_trim(writer.get_string()));
  }

  String Type::_type_name_unmodified_lazy_get() {
    StreamWriter writer;

    for (Ref<Node> child : get_children()) {
      if (Ref<Identifier> id = child->as<Identifier>()) {
        writer.write(id->name);
      } else if (Ref<Unsigned> u = child->as<Unsigned>()) {
        writer.write(" unsigned ");
      } else if (Ref<Signed> s = child->as<Signed>()) {
        writer.write(" signed ");
      } else if (Ref<Short> s = child->as<Short>()) {
        writer.write(" short ");
      } else if (Ref<Long> s = child->as<Long>()) {
        writer.write(" long ");
      }
    }

    return string_shrink_inner_space(string_trim(writer.get_string()));
  }

  Size Type::_template_argument_count_lazy_get() {
    if (!is_template_type()) {
      return 0;
    }
    return template_arguments()->get_child_count();
  }

  bool Type::_is_template_type_lazy_get() { return template_arguments() != nullptr; }

  Ref<TemplateArguments> Type::_template_arguments_lazy_get() { return find_child<TemplateArguments>(); }

}  // namespace GodotObjectCompiler
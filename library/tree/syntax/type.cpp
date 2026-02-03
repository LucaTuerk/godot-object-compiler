#include "type.h"

#include "identifier.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output_transformator.h"
#include "modifiers.h"

namespace GodotObjectCompiler {

  String Type::_type_name_lazy_get() {
    StreamWriter writer;
    OutputTransformator().transform(shared_from_this())->get_output(&writer);
    return string_shrink_inner_space(string_trim(writer.get_string()));
  }

  String Type::_type_name_untemplated_lazy_get() {
    StreamWriter writer;
    Ref<Type> temp = node_new<Type>();
    for (const Ref<Node>& child : *this) {
      if (child->is<TemplateParameters>() || child->is<TemplateArguments>()) {
        continue;
      }
      temp->add_child(child->clone());
    }

    OutputTransformator().transform(temp)->get_output(&writer);
    return string_shrink_inner_space(string_trim(writer.get_string()));
  }

  String Type::_type_name_unmodified_lazy_get() {
    StreamWriter writer;
    Ref<Type> temp = node_new<Type>();
    for (const Ref<Node>& child : *this) {
      if (child->is<TypeQualifier>() || child->is<Pointer>() || child->is<Reference>()) {
        continue;
      }

      temp->add_child(child->clone());
    }

    OutputTransformator().transform(temp)->get_output(&writer);
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
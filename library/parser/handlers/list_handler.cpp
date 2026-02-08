#include "list_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool ListHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    // TODO: Should the be string_suffix?
    return string_contains(p_current_src->type, "list");
  }

  ParserStep ListHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (r_current_target->is<Attribute>()) {
      // Parameters are handled by attributes parser.
      return ParserStep::StepOver();
    }

    if (p_current_src->type_in({"declaration_list", "field_declaration_list"})) {
      r_current_target = r_current_target->create_child<Body>();
    } else if (p_current_src->type == "parameter_list") {
      r_current_target = r_current_target->create_child<Parameters>();
    } else if (p_current_src->type == "template_argument_list") {
      r_current_target = r_current_target->create_child<TemplateArguments>();
    } else if (p_current_src->type == "template_parameter_list") {
      r_current_target = r_current_target->create_child<TemplateParameters>();
    } else {
      r_current_target = r_current_target->create_child<Context>();
    }

    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
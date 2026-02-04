#include "list_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  bool ListHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    // TODO: Should the be string_suffix?
    return string_contains(current_src->type, "list");
  }

  ParserStep ListHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_target->is<Attribute>()) {
      // Parameters are handled by attributes parser.
      return ParserStep::StepOver();
    }

    if (current_src->type_in({"declaration_list", "field_declaration_list"})) {
      current_target = current_target->create_child<Body>();
    } else if (current_src->type == "parameter_list") {
      current_target = current_target->create_child<Parameters>();
    } else if (current_src->type == "template_argument_list") {
      current_target = current_target->create_child<TemplateArguments>();
    } else {
      current_target = current_target->create_child<Context>();
    }

    return ParserStep::StepInto();
  }

}  // namespace GodotObjectCompiler
//
// Created by luca on 18.01.26.
//

#include "type_handler.h"

#include "library/core/helpers.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  // bool TypeHandler::handles_node(TSNode& node, const String& type) {
  //   return string_suffix(type, "type") || type == "qualified_identifier" || type == "type_identifier" ||
  //          type == "trailing_return_type";
  // }
  //
  // NextStep TypeHandler::handle(ParserContext& context) {
  //   if (context.current_node->is<Class>() || context.current_node->is<Struct>() || context.current_node->is<Enum>())
  //   {
  //     Ref<Identifier> class_name = context.current_node->create_child<Identifier>();
  //     class_name->name = context.copy_node_content(context.node);
  //     return STEP_OVER;
  //   }
  //
  //   String type = ts_node_type(context.node);
  //   if (type == "template_type") {
  //     context.current_node = context.current_node->create_child<Type>();
  //     return STEP_INTO;
  //   }
  //
  //   if (type == "template_argument_list") {
  //     context.current_node = context.current_node->create_child<TemplateArguments>();
  //     return STEP_INTO;
  //   }
  //
  //   Ref<Node> last = context.current_node->get_child(-1);
  //   bool last_is_qualifier = last && last->is<TypeQualifier>();
  //
  //   Ref<Type> type_node;
  //   if (context.current_node->is<Type>()) {
  //     type_node = context.current_node->as<Type>();
  //   } else {
  //     if (Ref<PlaceholderType> placeholder = context.current_node->find_child<PlaceholderType>()) {
  //       type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
  //       context.current_node->replace_child(placeholder, type_node);
  //     } else {
  //       type_node = context.current_node->create_child<Type>();
  //     }
  //   }
  //
  //   if (last_is_qualifier) {
  //     last->reparent(type_node);
  //   }
  //
  //   Ref<Identifier> type_name = type_node->create_child<Identifier>();
  //   type_name->name = context.copy_node_content(context.node);
  //
  //   return STEP_OVER;
  // }

  bool TypeHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_suffix(current_src->type, "type") ||
           current_src->type_in({"qualified_identifier", "type_identifier", "trailing_return_type"});
  }

  ParserStep TypeHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    if (current_target->is<Class>() || current_target->is<Struct>() || current_target->is<Enum>()) {
      current_target->create_child<Identifier>(current_src->content());
      return ParserStep::StepOver();
    }

    if (current_src->type == "template_type") {
      current_target = current_target->create_child<Type>();
      return ParserStep::StepInto();
    }

    if (current_src->type == "template_argument_list") {
      current_target = current_target->create_child<TemplateArguments>();
      return ParserStep::StepInto();
    }

    Ref<Node> last = current_target->get_child(-1);
    bool last_is_qualifier = last && last->is<TypeQualifier>();

    Ref<Type> type_node;
    if (current_target->is<Type>()) {
      type_node = current_target->as<Type>();
    } else {
      if (Ref<PlaceholderType> placeholder = current_target->find_child<PlaceholderType>()) {
        type_node = ExecutionContext::instance()->get_node_db()->create<Type>();
        current_target->replace_child(placeholder, type_node);
      } else {
        type_node = current_target->create_child<Type>();
      }
    }

    if (last_is_qualifier) {
      last->reparent(type_node);
    }

    type_node->create_child<Identifier>(current_src->content());
    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
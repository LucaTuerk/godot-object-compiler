//
// Created by luca on 18.01.26.
//

#include "identifier_handler.h"

#include "library/attribute_db.h"
#include "library/core/helpers.h"
#include "library/parser/attribute_argument_parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler {

  IdentifierHandler::IdentifierHandler() = default;

  bool IdentifierHandler::handles_node(TSNode& node, const String& type) {
    return string_contains(type, "identifier") && !(type == "qualified_identifier" || type == "type_identifier");
  }

  bool IdentifierHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_contains(current_src->type, "identifier") &&
           !(current_src->type == "qualified_identifier" || current_src->type == "type_identifier");
  }

  ParserStep IdentifierHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    Ref<Context> target = current_target;

    if (current_target->is<Field>() && current_target->empty()) {
      target = current_target->create_child<Field>();
    }

    target->create_child<Identifier>(current_src->content());

    if (AttributeDB::instance()->is_known_macro(current_src->content())) {
      return handle_known_attribute(current_src, current_target, current_src->content());
    }

    return ParserStep::StepOver();
  }

  ParserStep IdentifierHandlerV2::handle_known_attribute(
      const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target, const String& macro) {
    if (Ref<Context> parent = current_target->get_parent(); parent) {
      Ref<Attribute> attribute = AttributeDB::instance()->create_for_macro(macro);
      HANDLER_ERROR_COND_V2(!attribute, "Failed to create attribute for known macro %s", macro.c_str());

      parent->replace_child(current_target, attribute, true);
      current_target = parent;

      Ref<TreeSitterNode> ancestor =
          current_src->find_ancestor(DIRECT_PARENTS, type_in({"declaration", "expression_statement"}));

      HANDLER_ERROR_COND_V2(!ancestor, "Failed to create attribute for known macro %s", macro.c_str());
      attribute->start = ancestor->start_byte;
      attribute->end = ancestor->end_byte;
      attribute->line = ancestor->start_point.row + 1;

      Ref<IAttributeArgumentParser> argument_parser = attribute->get_argument_parser();
      if (argument_parser) {
        auto itr = current_src->context->stripped_parameters.find((Size)ts_node_start_byte(current_src->context->node));
        if (itr != current_src->context->stripped_parameters.end()) {
          Ref<ParserError> error =
              argument_parser->parse_attribute_arguments(itr->second, attribute->create_child<Arguments>());

          if (error != ParserError::OK) {
            attribute->remove_all_children();
            attribute->add_child(node_new<ParserError>(error->error_level, "TreeSitterParser",
                format("Failed to parse attribute arguments.\n%s", error->message.c_str()),
                current_src->context->file_path, current_src->context->original_buffer, attribute->line,
                ancestor->start_point.column + 1));
            error->set_handled();
          }
        }
      }

      return ParserStep::GoTo(ancestor);
    }
    return ParserStep::StepOver();
  }

  NextStep IdentifierHandler::handle_known_attribute(ParserContext& context, const String& macro) {
    if (Ref<Context> parent = context.current_node->get_parent(); parent) {
      Ref<Attribute> attribute = AttributeDB::instance()->create_for_macro(macro);
      HANDLER_ERROR_COND(!attribute, "Failed to create attribute for known macro %s", macro.c_str());

      parent->replace_child(context.current_node, attribute, true);

      context.current_node = attribute->get_parent();
      TSNode current = ts_tree_cursor_current_node(&context.cursor);
      TSNode ancestor = find_ancestor_of_type(current, "declaration");
      if (ts_node_is_null(ancestor)) {
        ancestor = find_ancestor_of_type(current, "expression_statement");
      }

      auto node_start_point = ts_node_start_point(ancestor);
      attribute->start = ts_node_start_byte(ancestor);
      attribute->end = ts_node_end_byte(ancestor);
      attribute->line = node_start_point.row + 1;

      Ref<IAttributeArgumentParser> argument_parser = attribute->get_argument_parser();

      if (argument_parser) {
        auto itr = context.stripped_parameters.find((Size)ts_node_start_byte(context.node));
        if (itr != context.stripped_parameters.end()) {
          Ref<ParserError> error =
              argument_parser->parse_attribute_arguments(itr->second, attribute->create_child<Arguments>());

          if (error != ParserError::OK) {
            attribute->remove_all_children();
            attribute->add_child(node_new<ParserError>(error->error_level, "TreeSitterParser",
                format("Failed to parse attribute arguments.\n%s", error->message.c_str()), context.file_path,
                context.original_buffer, attribute->line, node_start_point.column + 1));
            error->set_handled();
          }
        }
      }

      context.specific_step_id = ancestor.id;
      return STEP_OVER_SPECIFIC;
    }

    return STEP_OVER;
  }

  NextStep IdentifierHandler::handle(ParserContext& context) {
    Ref<Context> target = context.current_node;

    if (context.current_node->is<Field>() && context.current_node->get_child_count() == 0) {
      target = context.current_node->create_child<Type>();
    }

    Ref<Identifier> identifier = target->create_child<Identifier>();
    identifier->name = context.copy_node_content(context.node);

    if (AttributeDB::instance()->is_known_macro(identifier->name)) {
      return handle_known_attribute(context, identifier->name);
    }

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler

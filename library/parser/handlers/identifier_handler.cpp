//
// Created by luca on 18.01.26.
//

#include "identifier_handler.h"

#include "library/attribute_db.h"
#include "library/core/helpers.h"
#include "library/parser/attribute_argument_parser.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler {

  TSNode find_ancestor_of_type(TSNode node, const String& type) {
    TSNode ancestor = node;
    do {
      ancestor = ts_node_parent(ancestor);
    } while (!ts_node_is_null(ancestor) && ts_node_type(ancestor) != type);
    return ancestor;
  }

  IdentifierHandler::IdentifierHandler() = default;

  bool IdentifierHandler::handles_node(TSNode& node, const String& type) {
    return string_contains(type, "identifier") && !(type == "qualified_identifier" || type == "type_identifier");
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

}  // namespace GodotObjectCompiler
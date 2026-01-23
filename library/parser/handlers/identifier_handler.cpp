//
// Created by luca on 18.01.26.
//

#include "identifier_handler.h"

#include "library/core/helpers.h"
#include "library/parser/attribute_argument_parser.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler {

  TSNode find_ancestor_of_type(TSNode node, const String& type) {
    TSNode ancestor = node;
    do {
      ancestor = ts_node_parent(ancestor);
    } while (ts_node_type(ancestor) != type);
    return ancestor;
  }

  IdentifierHandler::IdentifierHandler() {
    _known_attributes.insert("GODOT_CLASS");
    _known_attributes.insert("GODOT_GENERATED_CLASS");
    _known_attributes.insert("GODOT_FUNCTION");
    _known_attributes.insert("GODOT_PROPERTY");
  }

  bool IdentifierHandler::handles_node(TSNode& node, const String& type) {
    return string_contains(type, "identifier") && !(type == "qualified_identifier" || type == "type_identifier");
  }

  NextStep IdentifierHandler::handle(ParserContext& context) {
    Context* target = context.current_node;

    if (dynamic_cast<Field*>(context.current_node) && context.current_node->get_child_count() == 0) {
      target = context.current_node->create_child<Type>();
    }

    Identifier* identifier = target->create_child<Identifier>();
    identifier->name = context.copy_node_content(context.node);

    if (AttributeDB::instance()->is_known_macro(identifier->name)) {
      return handle_known_attribute(context, identifier->name);
    }

    return STEP_OVER;
  }

  NextStep IdentifierHandler::handle_known_attribute(ParserContext& context, const String& macro) {
    if (!dynamic_cast<Function*>(context.current_node)) {
      return STEP_OVER;
    }

    if (Context* parent = context.current_node->get_parent(); parent) {
      Attribute* attribute = AttributeDB::instance()->create_for_macro(macro);
      parent->replace_child(context.current_node, attribute, true);

      context.current_node = attribute;

      auto itr = context.stripped_parameters.find((Size)ts_node_start_byte(context.node));
      if (itr != context.stripped_parameters.end()) {
        AttributeArgumentParser parser;
        attribute->add_child(parser.parse(itr->second));
        context.current_node = attribute->get_parent();
        TSNode current = ts_tree_cursor_current_node(&context.cursor);
        TSNode ancestor = find_ancestor_of_type(current, "declaration");
        context.specific_step_id = ancestor.id;
        return STEP_OVER_SPECIFIC;
      }
    }

    return STEP_OVER;
  }

}  // namespace GodotObjectCompiler
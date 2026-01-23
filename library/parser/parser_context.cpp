#include "library/parser/parser_context.h"

#include "../tree/syntax/all.h"

#define CREATE_NAMED(type)                               \
  type* result = current_node->create_child<type>();     \
  Identifier* name = result->create_child<Identifier>(); \
  name->name = get_child_content(node, "name");          \
  return result

namespace GodotObjectCompiler {

  String ParserContext::get_child_content(const TSNode& p_node, String p_child_name) {
    TSNode child = get_child_node(p_node, p_child_name);
    if (ts_node_is_null(child)) {
      print_err("Failed to get child node " + p_child_name);
      return String();
    }
    return copy_node_content(child);
  }

  TSNode ParserContext::get_child_node(const TSNode& p_node, String p_field_name) {
    return ts_node_child_by_field_name(p_node, p_field_name.c_str(), p_field_name.length());
  }

  TSNode ParserContext::get_child_node_by_type(const String& type, bool& success) {
    for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
      TSNode child = ts_node_child(node, i);
      if (ts_node_type(child) == type) {
        success = true;
        return child;
      }
    }
    success = false;
    return {};
  }

  TSNode _get_descendant_by_type_helper(TSNode node, const String& type, bool& success) {
    for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
      TSNode child = ts_node_child(node, i);
      if (ts_node_type(child) == type) {
        success = true;
        return child;
      }

      TSNode child_child = _get_descendant_by_type_helper(child, type, success);
      if (success) {
        return child_child;
      }
    }

    success = false;
    return {};
  }

  TSNode ParserContext::get_descendant_by_type(const String& type, bool& success) {
    return _get_descendant_by_type_helper(node, type, success);
  }

  String ParserContext::copy_node_content(const TSNode& p_node) {
    return buffer.substr(ts_node_start_byte(p_node), ts_node_end_byte(p_node) - ts_node_start_byte(p_node));
  }

  Namespace* ParserContext::create_namespace() {
    Namespace* result = current_node->create_child<Namespace>();
    return result;
  }

  Class* ParserContext::create_class() {
    Class* result = current_node->create_child<Class>();
    return result;
  }

  Struct* ParserContext::create_struct() {
    Struct* result = current_node->create_child<Struct>();
    return result;
  }

  Function* ParserContext::create_function() {
    Function* result = current_node->create_child<Function>();
    return result;
  }

  Field* ParserContext::create_field() {
    Field* result = current_node->create_child<Field>();
    return result;
  }

  bool ParserContext::is_valid() const {
    uint32_t node_child_count = ts_node_child_count(node);
    return !_invalid && node_child_count > 0;
  }

  ParserContext::ParserContext(const String& input) {
    buffer = input;
    global_namespace = ExecutionContext::instance()->get_node_db()->create<Namespace>();
    current_node = global_namespace;

    parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());
    tree = ts_parser_parse_string(parser, nullptr, buffer.c_str(), buffer.length());
    node = ts_tree_root_node(tree);
    cursor = ts_tree_cursor_new(node);
    _invalid = ts_node_is_null(node);
  }

  ParserContext::~ParserContext() {
    if (!_invalid) {
      // ts_tree_cursor_delete(&cursor);
      // ts_tree_delete(tree);
    }
  }

}  // namespace GodotObjectCompiler

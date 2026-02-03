#include "library/parser/parser_context.h"

#include "../tree/syntax/all.h"
#include "library/core/helpers.h"
#include "parser.h"
#include "tree_sitter_node.h"

#define CREATE_NAMED(type)                                   \
  type* result = current_node->create_child<type>();         \
  Ref<Identifier> name = result->create_child<Identifier>(); \
  name->name = get_child_content(node, "name");              \
  return result

namespace GodotObjectCompiler {

  // String ParserContext::get_child_content(const TSNode& p_node, String p_child_name) {
  //   TSNode child = get_child_node(p_node, p_child_name);
  //   if (ts_node_is_null(child)) {
  //     print_err("Failed to get child node " + p_child_name);
  //     return String();
  //   }
  //   return copy_node_content(child);
  // }
  //
  // TSNode ParserContext::get_child_node(const TSNode& p_node, String p_field_name) {
  //   return ts_node_child_by_field_name(p_node, p_field_name.c_str(), p_field_name.length());
  // }
  //
  // TSNode ParserContext::get_child_node_by_type(const String& type, bool& success) {
  //   for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
  //     TSNode child = ts_node_child(node, i);
  //     if (ts_node_type(child) == type) {
  //       success = true;
  //       return child;
  //     }
  //   }
  //   success = false;
  //   return {};
  // }
  //
  // TSNode _get_descendant_by_type_helper(TSNode node, const String& type, bool& success) {
  //   for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
  //     TSNode child = ts_node_child(node, i);
  //     if (ts_node_type(child) == type) {
  //       success = true;
  //       return child;
  //     }
  //
  //     TSNode child_child = _get_descendant_by_type_helper(child, type, success);
  //     if (success) {
  //       return child_child;
  //     }
  //   }
  //
  //   success = false;
  //   return {};
  // }
  //
  // TSNode ParserContext::get_descendant_by_type(const String& type, bool& success) {
  //   return _get_descendant_by_type_helper(node, type, success);
  // }
  //
  // String ParserContext::copy_node_content(const TSNode& p_node) {
  //   return buffer.substr(ts_node_start_byte(p_node), ts_node_end_byte(p_node) - ts_node_start_byte(p_node));
  // }

  bool ParserContext::is_valid() const { return current_src && !current_src->empty(); }

  ParserContext::ParserContext(const String& input) {
    buffer = input;
    current_target = ExecutionContext::instance()->get_node_db()->create<Namespace>();

    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree* tree = ts_parser_parse_string(parser, nullptr, buffer.c_str(), buffer.length());
    TSNode node = ts_tree_root_node(tree);

    _invalid = ts_node_is_null(node);
    current_src = _invalid ? nullptr : create_node(node);
    src_root = current_src;

    ts_tree_delete(tree);
    ts_parser_delete(parser);
  }

  ParserContext ParserContext::from_path(const String& path) {
    ParserContext result{read_file(path)};
    result.file_path = path;
    return result;
  }

  ParserContext::~ParserContext() {}

  Ref<TreeSitterNode> ParserContext::create_tree(TSTree* tree) {
    TSNode ts_root = ts_tree_root_node(tree);
    if (ts_node_is_null(ts_root)) {
      return nullptr;
    }

    return create_node(ts_root);
  }

  Ref<TreeSitterNode> ParserContext::create_node(TSNode ts_node) {
    if (ts_node_is_null(ts_node)) {
      return nullptr;
    }

    Ref<TreeSitterNode> node = node_new<TreeSitterNode>(ts_node, this);
    for (uint32_t i = 0; i < ts_node_child_count(ts_node); ++i) {
      auto child = create_node(ts_node_child(ts_node, i));
      if (child) {
        node->add_child(child);
      }
    }

    return node;
  }

}  // namespace GodotObjectCompiler

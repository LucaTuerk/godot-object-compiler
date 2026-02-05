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
    if (ts_node_is_null(ts_node) || String(ts_node_type(ts_node)) == "comment") {
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

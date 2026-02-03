
#include "tree_sitter_node.h"

#include "parser_context.h"

namespace GodotObjectCompiler {

  TreeSitterNode::Point::Point(const TSPoint& point) {
    row = point.row;
    column = point.column;
  }

  TreeSitterNode::TreeSitterNode(const TSNode& node, ParserContext* parser_context) {
    context = parser_context;
    type = ts_node_type(node);
    start_byte = ts_node_start_byte(node);
    end_byte = ts_node_end_byte(node);
    start_point = ts_node_start_point(node);
    end_point = ts_node_end_point(node);
  }

  String TreeSitterNode::to_string() const { return type; }

  bool TreeSitterNode::type_in(std::initializer_list<String>&& type_names) const {
    return std::find(type_names.begin(), type_names.end(), type) != type_names.end();
  }

  String TreeSitterNode::_content_lazy_get() { return context->buffer.substr(start_byte, end_byte - start_byte); }

}  // namespace GodotObjectCompiler
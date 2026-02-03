
#include "tree_sitter_node.h"

namespace GodotObjectCompiler {

  TreeSitterNode::Point::Point(const TSPoint& point) {
    row = point.row;
    column = point.column;
  }

  TreeSitterNode::TreeSitterNode(const TSNode& node) {
    type = ts_node_type(node);
    start_byte = ts_node_start_byte(node);
    end_byte = ts_node_end_byte(node);
    start_point = ts_node_start_point(node);
    end_point = ts_node_end_point(node);
  }

  String TreeSitterNode::to_string() const { return type; }

}  // namespace GodotObjectCompiler

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

  bool TreeSitterNode::type_in(std::initializer_list<String>&& p_type_names) const {
    return std::find(p_type_names.begin(), p_type_names.end(), type) != p_type_names.end();
  }

  bool TreeSitterNode::is_handled() const { return handled; }

  void TreeSitterNode::set_handled(bool p_handled) { this->handled = p_handled; }

  bool TreeSitterNode::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(TreeSitterNode, Context)

    target->context = context;
    target->type = type;
    target->start_byte = start_byte;
    target->end_byte = end_byte;
    target->start_point = start_point;
    target->end_point = end_point;
    return true;
  }

  String TreeSitterNode::_content_lazy_get() { return context->buffer.substr(start_byte, end_byte - start_byte); }

}  // namespace GodotObjectCompiler
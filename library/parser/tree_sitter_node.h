
#pragma once
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

#include "library/core/helpers.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  struct ParserContext;

  class TreeSitterNode : public Context {
    NODE_TYPE(TreeSitterNode);

    struct Point {
      Size row = 0;
      Size column = 0;

      Point() = default;
      Point(const TSPoint& point);
    };

    TreeSitterNode(const TSNode& node);

    String to_string() const override;

    bool type_in(std::initializer_list<String>&& type_names) const;

    ParserContext* context;
    String type;
    Size start_byte;
    Size end_byte;
    Point start_point;
    Point end_point;

    LAZY(TreeSitterNode, String, content);
  };

  inline Predicate<TreeSitterNode> type_is(const String& type) {
    return [type](Ref<TreeSitterNode> node) { return node->type == type; };
  }

  inline Predicate<TreeSitterNode> type_in(std::initializer_list<String>&& types) {
    return
        [types](Ref<TreeSitterNode> node) { return std::find(types.begin(), types.end(), node->type) != types.end(); };
  }

  inline Predicate<TreeSitterNode> type_contains(const String& str) {
    return [str](Ref<TreeSitterNode> node) { return string_contains(node->type, str); };
  }

}  // namespace GodotObjectCompiler

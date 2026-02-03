#pragma once

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

#include "../tree/syntax/node.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

  class TreeSitterNode;

  class Node;
  class Namespace;
  class Class;
  class Struct;
  class Function;
  class Field;

  struct ParserContext {
    using NodeID = const void*;

    String file_path;
    String original_buffer;
    String buffer;
    Ref<Context> current_node;
    TSParser* parser;
    TSTree* tree;
    TSTreeCursor cursor;
    TSNode node;
    Ref<TreeSitterNode> ts_node;
    Ref<Namespace> global_namespace;
    Dictionary<Size, String> stripped_parameters;
    NodeID specific_step_id;

    String get_child_content(const TSNode& p_node, String p_child_name);
    TSNode get_child_node(const TSNode& p_node, String p_field_name);
    TSNode get_child_node_by_type(const String& type, bool& success);
    TSNode get_descendant_by_type(const String& type, bool& success);
    String copy_node_content(const TSNode& p_node);

    bool is_valid() const;

    ParserContext() = default;
    explicit ParserContext(const String& input);

    static ParserContext from_path(const String& path);
    ~ParserContext();

    static Ref<TreeSitterNode> create_tree(TSTree* tree);
    static Ref<TreeSitterNode> create_node(TSNode ts_node);

   private:

    bool _invalid = true;
  };

}  // namespace GodotObjectCompiler

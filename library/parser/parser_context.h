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
    Ref<Context> current_target;
    Ref<TreeSitterNode> src_root;
    Ref<TreeSitterNode> current_src;
    Dictionary<Size, String> stripped_parameters;

    bool is_valid() const;

    ParserContext() = default;
    explicit ParserContext(const String& input);

    static ParserContext from_path(const String& p_path);
    ~ParserContext();

    Ref<TreeSitterNode> create_tree(TSTree* p_tree);
    Ref<TreeSitterNode> create_node(TSNode p_ts_node);

   private:

    bool _invalid = true;
  };

}  // namespace GodotObjectCompiler

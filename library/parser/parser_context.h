#pragma once

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

#include "library/core/core.h"

namespace GodotObjectCompiler {
  class Context;
  class Namespace;
  class Class;
  class Struct;
  class Function;
  class Field;

  struct ParserContext {
    String buffer;
    Context* current_node;
    TSParser* parser;
    TSTree* tree;
    TSTreeCursor cursor;
    TSNode node;
    Namespace* global_namespace;

    String get_child_content(const TSNode& p_node, String p_child_name);
    TSNode get_child_node(const TSNode& p_node, String p_field_name);
    String copy_node_content(const TSNode& p_node);

    Namespace* create_namespace();
    Class* create_class();
    Struct* create_struct();
    Function* create_function();
    Field* create_field();

    bool is_valid() const;

    ParserContext() = default;
    explicit ParserContext(const String& input);
    ~ParserContext();

   private:
    bool _invalid = true;
  };
}  // namespace GodotObjectCompiler

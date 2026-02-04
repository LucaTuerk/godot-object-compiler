#include "pointer_ref_handler.h"

#include "library/core/helpers.h"
#include "library/parser/node_handler.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  bool PointerRefHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return string_only_contains(current_src->type, '&') || string_only_contains(current_src->type, '*');
  }

  ParserStep PointerRefHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    enum Option { REFERENCE, POINTER };

    Ref<Context> target_node = current_target->find_child<Type>();
    if (!target_node) {
      target_node = current_target;
    }

    Option option = string_only_contains(current_src->type, '&') ? REFERENCE : POINTER;
    for (Size i = 0; i < current_src->type.length(); ++i) {
      switch (option) {
        case REFERENCE:
          target_node->create_child<Reference>();
          break;
        case POINTER:
          target_node->create_child<Pointer>();
          break;
      }
    }

    return ParserStep::Undecided();
  }

}  // namespace GodotObjectCompiler
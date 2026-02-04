//
// Created by luca on 17.01.26.
//

#include "access_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/access_specifier.h"

namespace GodotObjectCompiler {

  bool AccessSpecifierHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "access_specifier";
  }

  ParserStep AccessSpecifierHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    const String content = current_src->content();

    if (content == "public") {
      current_target->build_child<AccessSpecifier>(AccessSpecifier::PUBLIC);
    } else if (content == "private") {
      current_target->build_child<AccessSpecifier>(AccessSpecifier::PRIVATE);
    } else if (content == "protected") {
      current_target->build_child<AccessSpecifier>(AccessSpecifier::PROTECTED);
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
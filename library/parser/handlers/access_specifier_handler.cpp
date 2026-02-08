//
// Created by luca on 17.01.26.
//

#include "access_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/access_specifier.h"
#include "library/tree/syntax/class.h"

namespace GodotObjectCompiler {

  bool AccessSpecifierHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "access_specifier";
  }

  ParserStep AccessSpecifierHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (r_current_target->is<BaseClasses>()) {
      // We don't care about base class access specifiers for now, so keep it clean an skip here.
      return ParserStep::StepOver();
    }

    const String content = p_current_src->content();

    if (content == "public") {
      r_current_target->build_child<AccessSpecifier>(AccessSpecifier::PUBLIC);
    } else if (content == "private") {
      r_current_target->build_child<AccessSpecifier>(AccessSpecifier::PRIVATE);
    } else if (content == "protected") {
      r_current_target->build_child<AccessSpecifier>(AccessSpecifier::PROTECTED);
    }

    return ParserStep::StepOver();
  }

}  // namespace GodotObjectCompiler
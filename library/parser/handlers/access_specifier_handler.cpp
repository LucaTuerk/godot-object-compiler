//
// Created by luca on 17.01.26.
//

#include "access_specifier_handler.h"

#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  bool AccessSpecifierHandler::handles_node(TSNode& node, const String& type) {
    return String(ts_node_type(node)) == "access_specifier";
  }

  NextStep AccessSpecifierHandler::handle(ParserContext& context) {
    const String value = context.copy_node_content(context.node);
    Ref<AccessSpecifier> as = context.current_node->create_child<AccessSpecifier>();

    if (value == "public") {
      as->type = AccessSpecifier::PUBLIC;
    } else if (value == "protected") {
      as->type = AccessSpecifier::PROTECTED;
    } else if (value == "private") {
      as->type = AccessSpecifier::PRIVATE;
    }

    return STEP_OVER;
  }

  bool AccessSpecifierHandlerV2::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == "access_specifier";
  }

  ParserStep AccessSpecifierHandlerV2::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
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
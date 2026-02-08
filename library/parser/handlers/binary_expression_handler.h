
#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class EnumValue;

  class BinaryExpressionHandler : public INodeHandler {
    NODE_HANDLER(BinaryExpressionHandler)
   public:

    bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) override;

    static Ref<int> calculate_binary_expression(const Ref<TreeSitterNode>& left_operand,
        const Ref<TreeSitterNode>& expr_operator, const Ref<TreeSitterNode>& right_operand,
        const Ref<EnumValue>& current_target);

    static Ref<int> find_value_for_identifier(const String& identifier, const Ref<EnumValue>& current_target);
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  class FieldHandler : public INodeHandler {
    NODE_HANDLER(FieldHandler)

   public:

    ~FieldHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;
  };

  class FieldHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(FieldHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  class ExpressionStatementIntoV2 : public IntoHandler<ExpressionStatementIntoV2> {
    NODE_HANDLER_V2(ExpressionStatementIntoV2);

   public:

    static inline String into_type = "expression_statement";
  };

  INTO(expression_statement);

}  // namespace GodotObjectCompiler

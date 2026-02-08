#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class FieldHandler : public INodeHandler {
    NODE_HANDLER(FieldHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) override;
  };

  class ExpressionStatementInto : public IntoHandler<ExpressionStatementInto> {
    NODE_HANDLER(ExpressionStatementInto);

   public:

    static inline String into_type = "expression_statement";
  };

}  // namespace GodotObjectCompiler

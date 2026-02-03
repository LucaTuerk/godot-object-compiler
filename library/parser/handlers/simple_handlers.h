#pragma once

#include "../node_handler.h"
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  // DO NOT IMPLEMENT
  SKIP(template_declaration);
  SKIP(conditional_expression);
  SKIP(compound_statement);

  class TemplateDeclarationSkipV2 : public SkipHandler<TemplateDeclarationSkipV2> {
    NODE_HANDLER_V2(TemplateDeclarationSkipV2);

   public:

    static inline String skip_type = "template_declaration";
  };

  class ConditionalExpressionSkipV2 : public SkipHandler<ConditionalExpressionSkipV2> {
    NODE_HANDLER_V2(ConditionalExpressionSkipV2);

   public:

    static inline String skip_type = "conditional_expression";
  };

  class CompoundStatementSkipV2 : public SkipHandler<CompoundStatementSkipV2> {
    NODE_HANDLER_V2(CompoundStatementSkipV2);

   public:

    static inline String skip_type = "compound_statement";
  };

  // MAYBE IMPLEMENT
  SKIP(number_literal);

  class NumberLiteralSkipV2 : public SkipHandler<NumberLiteralSkipV2> {
    NODE_HANDLER_V2(NumberLiteralSkipV2);

   public:

    static inline String skip_type = "number_literal";
  };

  // SKIP(call_expression);
  SKIP(preproc_call);

  class PreprocCallSkipV2 : public SkipHandler<PreprocCallSkipV2> {
    NODE_HANDLER_V2(PreprocCallSkipV2);

   public:

    static inline String skip_type = "preproc_call";
  };

  // SKIP(preproc_def);
  INTO(preproc_ifdef);

  class PreprocIfDefIntoV2 : public IntoHandler<PreprocIfDefIntoV2> {
    NODE_HANDLER_V2(PreprocIfDefIntoV2);

   public:

    static inline String into_type = "preproc_if_def_into";
  };

  // SKIP(preproc_function_def);

  SKIP(operator);

  class OperatorSkipV2 : public SkipHandler<OperatorSkipV2> {
    NODE_HANDLER_V2(OperatorSkipV2);

   public:

    static inline String skip_type = "operator";
  };

}  // namespace GodotObjectCompiler

#pragma once

#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class TemplateDeclarationInto : public IntoHandler<TemplateDeclarationInto> {
    NODE_HANDLER(TemplateDeclarationInto);

   public:

    static inline String into_type = "template_declaration";
  };

  class ConditionalExpressionSkip : public SkipHandler<ConditionalExpressionSkip> {
    NODE_HANDLER(ConditionalExpressionSkip);

   public:

    static inline String skip_type = "conditional_expression";
  };

  class CompoundStatementSkip : public SkipHandler<CompoundStatementSkip> {
    NODE_HANDLER(CompoundStatementSkip);

   public:

    static inline String skip_type = "compound_statement";
  };

  class NumberLiteralSkip : public SkipHandler<NumberLiteralSkip> {
    NODE_HANDLER(NumberLiteralSkip);

   public:

    static inline String skip_type = "number_literal";
  };

  class PreprocCallSkip : public SkipHandler<PreprocCallSkip> {
    NODE_HANDLER(PreprocCallSkip);

   public:

    static inline String skip_type = "preproc_call";
  };

  //
  // class PreprocIfDefInto : public IntoHandler<PreprocIfDefInto> {
  //   NODE_HANDLER(PreprocIfDefInto);
  //
  //  public:
  //
  //   static inline String into_type = "preproc_ifdef";
  // };

  class OperatorSkip : public SkipHandler<OperatorSkip> {
    NODE_HANDLER(OperatorSkip);

   public:

    static inline String skip_type = "operator";
  };

}  // namespace GodotObjectCompiler

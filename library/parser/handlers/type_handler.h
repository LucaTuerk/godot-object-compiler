#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  //
  // class TypeHandler : public INodeHandler {
  //   NODE_HANDLER(TypeHandler)
  //
  //  public:
  //
  //   ~TypeHandler() override = default;
  //   bool handles_node(TSNode& node, const String& type) override;
  //   NextStep handle(ParserContext& context) override;
  // };

  // INTO(trailing_return_type);
  // INTO(type_descriptor);
  // SIMPLE_GENERATE(placeholder_type_specifier, PlaceholderType, STEP_OVER);

  class TrailingReturnTypeIntoV2 : public IntoHandler<TrailingReturnTypeIntoV2> {
    NODE_HANDLER_V2(TrailingReturnTypeIntoV2);

   public:

    static inline String into_type = "trailing_return_type";
  };

  class TypeDescriptorIntoV2 : public IntoHandler<TypeDescriptorIntoV2> {
    NODE_HANDLER_V2(TypeDescriptorIntoV2);

   public:

    static inline String into_type = "type_descriptor";
  };

  class PlaceholderTypeGenerateV2 : public GenerateHandler<PlaceholderTypeGenerateV2, PlaceholderType> {
    NODE_HANDLER_V2(PlaceholderTypeGenerateV2);

   public:

    static inline String generate_type = "placeholder_type_generate";
    static inline ParserStep next_step = ParserStep::StepOver();
  };

  class TypeHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(TypeHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

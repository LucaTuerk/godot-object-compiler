#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  class TrailingReturnTypeInto : public IntoHandler<TrailingReturnTypeInto> {
    NODE_HANDLER(TrailingReturnTypeInto);

   public:

    static inline String into_type = "trailing_return_type";
  };

  class TypeDescriptorInto : public IntoHandler<TypeDescriptorInto> {
    NODE_HANDLER(TypeDescriptorInto);

   public:

    static inline String into_type = "type_descriptor";
  };

  class PlaceholderTypeGenerate : public GenerateHandler<PlaceholderTypeGenerate, PlaceholderType> {
    NODE_HANDLER(PlaceholderTypeGenerate);

   public:

    static inline String generate_type = "placeholder_type_generate";
    static inline ParserStep next_step = ParserStep::StepOver();
  };

  class TypeHandler : public INodeHandler {
    NODE_HANDLER(TypeHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  // class PointerRefHandler : public INodeHandler {
  //   NODE_HANDLER(PointerRefHandler)
  //
  //  public:
  //
  //   ~PointerRefHandler() override = default;
  //   bool handles_node(TSNode& node, const String& type) override;
  //   NextStep handle(ParserContext& context) override;
  //
  //  private:
  //
  //   enum Current { NONE, POINTER, REFERENCE };
  //
  //   Current _current;
  // };

  // INTO(reference_declarator);
  // INTO(abstract_reference_declarator);
  // INTO(pointer_declarator);
  // INTO(abstract_pointer_declarator);

  class ReferenceDeclaratorIntoV2 : public IntoHandler<ReferenceDeclaratorIntoV2> {
    NODE_HANDLER_V2(ReferenceDeclaratorIntoV2);

   public:

    static inline String into_type = "reference_declarator";
  };

  class AbstractRefereanceDeclaratorIntoV2 : public IntoHandler<AbstractRefereanceDeclaratorIntoV2> {
    NODE_HANDLER_V2(AbstractRefereanceDeclaratorIntoV2);

   public:

    static inline String into_type = "abstract_reference_declarator";
  };

  class PointerDeclaratorIntoV2 : public IntoHandler<PointerDeclaratorIntoV2> {
    NODE_HANDLER_V2(PointerDeclaratorIntoV2);

   public:

    static inline String into_type = "pointer_declarator";
  };

  class AbstractPointerDeclaratorIntoV2 : public IntoHandler<AbstractPointerDeclaratorIntoV2> {
    NODE_HANDLER_V2(PointerDeclaratorIntoV2);

   public:

    static inline String into_type = "abstract_pointer_declarator";
  };

  class PointerRefHandlerV2 : public INodeHandlerV2 {
    NODE_HANDLER_V2(PointerRefHandlerV2);

   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

}  // namespace GodotObjectCompiler
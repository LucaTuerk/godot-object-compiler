#pragma once
#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

  class ReferenceDeclaratorInto : public IntoHandler<ReferenceDeclaratorInto> {
    NODE_HANDLER(ReferenceDeclaratorInto);

   public:

    static inline String into_type = "reference_declarator";
  };

  class AbstractRefereanceDeclaratorInto : public IntoHandler<AbstractRefereanceDeclaratorInto> {
    NODE_HANDLER(AbstractRefereanceDeclaratorInto);

   public:

    static inline String into_type = "abstract_reference_declarator";
  };

  class PointerDeclaratorInto : public IntoHandler<PointerDeclaratorInto> {
    NODE_HANDLER(PointerDeclaratorInto);

   public:

    static inline String into_type = "pointer_declarator";
  };

  class AbstractPointerDeclaratorInto : public IntoHandler<AbstractPointerDeclaratorInto> {
    NODE_HANDLER(PointerDeclaratorInto);

   public:

    static inline String into_type = "abstract_pointer_declarator";
  };

  class PointerRefHandler : public INodeHandler {
    NODE_HANDLER(PointerRefHandler);

   public:

    bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) override;
  };

}  // namespace GodotObjectCompiler
#pragma once
#include "library/parser/node_handler.h"

namespace GodotObjectCompiler {

  INTO(reference_declarator);
  INTO(abstract_reference_declarator);
  INTO(pointer_declarator);
  INTO(abstract_pointer_declarator);

  class PointerRefHandler : public INodeHandler {
    NODE_HANDLER(PointerRefHandler)

   public:

    ~PointerRefHandler() override = default;
    bool handles_node(TSNode& node, const String& type) override;
    NextStep handle(ParserContext& context) override;

   private:

    enum Current { NONE, POINTER, REFERENCE };

    Current _current;
  };

}  // namespace GodotObjectCompiler
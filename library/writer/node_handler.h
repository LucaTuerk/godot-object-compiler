#pragma once
#include "library/tree/node.h"
#include "writer_context.h"

namespace GodotObjectCompiler {
  class Node;
  namespace Writer {

    class INodeHandler {
     public:
      virtual bool handles_node(Node* node) = 0;
      virtual bool handle(WriterContext& context);
    };

  }  // namespace Writer
}  // namespace GodotObjectCompiler

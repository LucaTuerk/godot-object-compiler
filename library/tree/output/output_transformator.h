#pragma once
#include "output.h"

namespace GodotObjectCompiler {

  class Node;

  class IOutputTransformator {
   public:

    virtual Ref<Writer::IOutputNode> transform(Ref<Node> tree) = 0;
  };

  class OutputTransformator : public IOutputTransformator {
   public:

    Ref<Writer::IOutputNode> transform(Ref<Node> tree) override;
  };

}  // namespace GodotObjectCompiler

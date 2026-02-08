#pragma once
#include "output.h"

namespace GodotObjectCompiler {

  class Node;

  class IOutputTransformator {
   public:

    virtual ~IOutputTransformator() = default;

    virtual Ref<Writer::IOutputNode> transform(Ref<Node> p_tree) = 0;
  };

  class OutputTransformator : public IOutputTransformator {
   public:

    Ref<Writer::IOutputNode> transform(Ref<Node> p_tree) override;

    void replace_non_output_children(Ref<Writer::IOutputNode> p_node);
  };

}  // namespace GodotObjectCompiler

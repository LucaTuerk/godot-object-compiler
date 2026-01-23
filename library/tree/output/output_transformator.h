#pragma once
#include "output.h"

namespace GodotObjectCompiler {

  class Node;

  class IOutputTransformator {
   public:

    virtual Writer::IOutputNode* transform(Node* tree) = 0;
  };

  class OutputTransformator : public IOutputTransformator {
   public:

    Writer::IOutputNode* transform(Node* tree) override;
  };

}  // namespace GodotObjectCompiler

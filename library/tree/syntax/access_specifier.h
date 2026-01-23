#pragma once
#include "library//core/config.h"
#include "node.h"

namespace GodotObjectCompiler {

  class AccessSpecifier : public Node {
    NODE_TYPE(AccessSpecifier);

    bool copy_to(Node* other) const override;
    String to_string() const override;
    void write_to(IStructuredWriter* writer) override;

    enum Type { PUBLIC, PRIVATE, PROTECTED };
    Type type = PUBLIC;
  };
}  // namespace GodotObjectCompiler

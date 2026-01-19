#pragma once

#include "node.h"

namespace GodotObjectCompiler {
  class Identifier : public Node {
    NODE_TYPE(Identifier);

    bool copy_to(Node* other) const override;
    String to_string() const override;
    virtual void write_to(IWriter* writer) override;
    String name;
  };
}  // namespace GodotObjectCompiler

#pragma once

#include "node.h"

namespace GodotObjectCompiler {
  class Identifier : public Node {
    NODE_TYPE(Identifier);

   public:
    explicit Identifier(const String& name) : name(name) {}
    bool copy_to(Node* other) const override;
    String to_string() const override;
    void write_to(IStructuredWriter* writer) override;
    void read_from(IStructuredReader* reader) override;
    String name;
  };
}  // namespace GodotObjectCompiler

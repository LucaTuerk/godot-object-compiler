#pragma once
#include "node.h"

namespace GodotObjectCompiler {

  class Literal : public Node {
    NODE_TYPE(Literal)

    explicit Literal(const String& content) : content(content) {}

    String to_string() const override;
    bool copy_to(Node* other) const override;
    void write_to(IStructuredWriter* writer) override;
    void read_from(IStructuredReader* reader) override;

    String content;
  };

}  // namespace GodotObjectCompiler

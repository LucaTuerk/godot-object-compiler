#pragma once
#include "node.h"

namespace GodotObjectCompiler {

  class Literal : public Node {
    NODE_TYPE(Literal)

    explicit Literal(const String& content) : content(content) {}

    static Ref<Literal> StringLiteral(const String& content);

    String to_string() const override;
    bool copy_to(Ref<Node> p_other) const override;
    void write_to(IStructuredWriter* p_writer) override;
    void read_from(IStructuredReader* p_reader) override;

    String content;
  };

}  // namespace GodotObjectCompiler

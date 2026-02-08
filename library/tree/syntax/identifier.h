#pragma once

#include "node.h"

namespace GodotObjectCompiler {

  class Identifier : public Node {
    NODE_TYPE(Identifier);

   public:

    explicit Identifier(const String& name) : name(name) {}

    bool copy_to(Ref<Node> p_other) const override;
    String to_string() const override;
    void write_to(IStructuredWriter* p_writer) override;
    void read_from(IStructuredReader* p_reader) override;
    String name;
  };

}  // namespace GodotObjectCompiler

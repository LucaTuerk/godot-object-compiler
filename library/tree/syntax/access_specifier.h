#pragma once
#include "library//core/config.h"
#include "node.h"

namespace GodotObjectCompiler {

  class AccessSpecifier : public Node {
    NODE_TYPE(AccessSpecifier);

    enum Type { PUBLIC, PRIVATE, PROTECTED };

    explicit AccessSpecifier(Type type) : type(type) {}

    bool copy_to(Ref<Node> other) const override;
    String to_string() const override;
    void write_to(IStructuredWriter* writer) override;

    Type type = PUBLIC;
  };

}  // namespace GodotObjectCompiler

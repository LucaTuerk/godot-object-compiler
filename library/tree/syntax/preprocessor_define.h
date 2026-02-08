#pragma once
#include "node.h"

namespace GodotObjectCompiler {

  class PreprocessorDefine : public Node {
    NODE_TYPE(PreprocessorDefine);

    explicit PreprocessorDefine(const String& name) : name(name) {}

    String to_string() const override;
    bool copy_to(Ref<Node> p_other) const override;
    void write_to(IStructuredWriter* p_writer) override;
    void read_from(IStructuredReader* p_reader) override;

    String name;
  };

}  // namespace GodotObjectCompiler

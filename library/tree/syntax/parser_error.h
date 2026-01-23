#pragma once
#include "library/core/core.h"
#include "library/core/reader_writer.h"
#include "node.h"

namespace GodotObjectCompiler {

  class ParserError : public Node {
    NODE_TYPE(ParserError)

    explicit ParserError(const String& offending) : offending(offending) {}

    String to_string() const override;
    bool copy_to(Node* other) const override;
    void write_to(IStructuredWriter* writer) override;
    void read_from(IStructuredReader* reader) override;

    String offending;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "context.h"
#include "library/core/lazy.h"

namespace GodotObjectCompiler {

  class Include : public Context {
    NODE_TYPE(Include);

    Include(const String& include_path, bool is_system_include)
        : is_system_include(is_system_include), include_path(include_path) {}

    enum Error { OK, ALREADY_INCLUDED, FILE_NOT_FOUND, PARSE_ERROR };

    String to_string() const override;
    void read_from(IStructuredReader* reader) override;
    bool copy_to(Node* other) const override;
    void write_to(IStructuredWriter* writer) override;

    LAZY(Include, Error, evaluate);

    bool is_system_include;
    String include_path;
  };

}  // namespace GodotObjectCompiler

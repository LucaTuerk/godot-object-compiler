#pragma once
#include <utility>

#include "../syntax/function.h"
#include "../syntax/node.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {
  namespace Writer {

    class IOutputNode;

  }

  class OutputFile : public Context {
    NODE_TYPE(OutputFile);

    explicit OutputFile(String path) : path(std::move(path)) {}

    void print();
    void write_output(IStringWriter* writer);

    String path;
    Ref<Writer::IOutputNode> transformed;
  };

}  // namespace GodotObjectCompiler

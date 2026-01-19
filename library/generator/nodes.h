#pragma once
#include <utility>

#include "library/tree/function.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {

  class OutputFile : public Context {
    NODE_TYPE(OutputFile);
    explicit OutputFile(String path) : path(std::move(path)) {}

    String path;
  };

  class FunctionCall : public Node {
    NODE_TYPE(FunctionCall);
  };

  class Comment : public Node {
    NODE_TYPE(Comment);
    explicit Comment(String content) : content(std::move(content)) {}

    String content;
  };

  class StringLiteral : public Node {
    NODE_TYPE(StringLiteral);
    explicit StringLiteral(String content) : content(std::move(content)) {}

    String content;
  };

}  // namespace GodotObjectCompiler

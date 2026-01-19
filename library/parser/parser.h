#pragma once

#include "library/core/core.h"
#include "library/parser/parser_context.h"
#include "library/tree/node.h"

namespace GodotObjectCompiler {
  class INodeHandler;
  class ParserContext;

  class IParser {
   public:
    virtual Node* parse(const String& input) = 0;
  };

  class TreeSitterParser : public IParser {
   public:
    ~TreeSitterParser() = default;

    Node* parse(const String& input) override;

    template <typename T>
    static bool register_handler();

   private:
    static inline Vector<INodeHandler*> _handlers{};
    ParserContext context{};
  };

  template <typename T>
  bool TreeSitterParser::register_handler() {
    _handlers.push_back(new T());
    return true;
  }
}  // namespace GodotObjectCompiler

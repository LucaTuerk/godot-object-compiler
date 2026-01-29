#pragma once

#include "../tree/syntax/node.h"
#include "library/core/core.h"
#include "library/parser/parser_context.h"

namespace GodotObjectCompiler {

  class INodeHandler;
  class ParserContext;

  class IParser {
   public:

    virtual ~IParser() = default;
    virtual Ref<Node> parse(const String& input) = 0;
  };

  class TreeSitterParser : public IParser {
   public:

    ~TreeSitterParser() = default;

    Ref<Node> parse_file(const String& path);
    Ref<Node> parse(const String& input) override;
    Ref<Node> parse(const String& input, std::vector<INodeHandler*> handlers);

    template <typename T>
    static bool register_handler();

   private:

    String strip_known_macro_contents(const String& input, Dictionary<Size, String>& parameters);

    bool input_is_path = false;
    static inline Vector<INodeHandler*> _handlers{};
    ParserContext context{};
  };

  template <typename T>
  bool TreeSitterParser::register_handler() {
    _handlers.push_back(new T());
    return true;
  }

}  // namespace GodotObjectCompiler

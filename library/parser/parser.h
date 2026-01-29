#pragma once

#include "../tree/syntax/node.h"
#include "library/core/core.h"
#include "library/parser/parser_context.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  class INodeHandler;
  class ParserContext;

  class IParser {
   public:

    virtual ~IParser() = default;
    virtual Ref<ParserError> parse(const String& input, Ref<Context> target) = 0;
  };

  class TreeSitterParser : public IParser {
   public:

    ~TreeSitterParser() = default;

    Ref<ParserError> parse_file(const String& path, Ref<Context> target);
    Ref<ParserError> parse(const String& input, Ref<Context> target) override;

    template <typename T>
    static bool register_handler();

    static String strip_known_macro_contents(const String& input, Dictionary<Size, String>& parameters);

   private:

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

#define HANDLER_ERROR(message)                                    \
  node_new<ParserError>(ERROR, "TreeSitterParser: " + (message)); \
  return STEP_OVER;

#define HANDLER_ERROR_COND(condition, message) \
  if ((condition)) {                           \
    HANDLER_ERROR(message)                     \
  }

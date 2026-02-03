#pragma once

#include "../tree/syntax/node.h"
#include "library/core/core.h"
#include "library/parser/parser_context.h"
#include "library/tree/syntax/parser_error.h"

// #include "node_handler.h"

namespace GodotObjectCompiler {

  class INodeHandlerV2;

  class TreeSitterNode;

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

    template <typename T>
    static bool register_handler_v2(const String& name);

    static String strip_known_macro_contents(const String& input, Dictionary<Size, String>& parameters);

   private:

    bool input_is_path = false;
    static inline Vector<INodeHandler*> _handlers{};
    static inline HashSet<String> _handler_names_v2{};
    static inline Vector<Ref<INodeHandlerV2>> _handlers_v2{};
    // ParserContext context{};
  };

  template <typename T>
  bool TreeSitterParser::register_handler() {
    _handlers.push_back(new T());
    return true;
  }

  template <typename T>
  bool TreeSitterParser::register_handler_v2(const String& name) {
    if (_handler_names_v2.find(name) != _handler_names_v2.end()) {
      return false;
    }
    _handler_names_v2.insert(name);
    _handlers_v2.push_back(make_ref<T>());
    return true;
  }

}  // namespace GodotObjectCompiler

#define HANDLER_ERROR(...)                           \
  node_new<ParserError>(ERROR, format(__VA_ARGS__)); \
  return STEP_OVER;

#define HANDLER_ERROR_COND(condition, ...) \
  if ((condition)) {                       \
    HANDLER_ERROR(__VA_ARGS__)             \
  }

#define HANDLER_ERROR_V2(...)                        \
  node_new<ParserError>(ERROR, format(__VA_ARGS__)); \
  return ParserStep::StepOver();

#define HANDLER_ERROR_COND_V2(condition, ...) \
  if ((condition)) {                          \
    HANDLER_ERROR_V2(__VA_ARGS__)             \
  }

#pragma once

#include "../tree/syntax/node.h"
#include "library/core/core.h"
#include "library/parser/parser_context.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  class INodeHandler;
  class TreeSitterNode;
  class ParserContext;

  class IParser {
   public:

    virtual ~IParser() = default;
    virtual Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) = 0;
  };

  class TreeSitterParser : public IParser {
   public:

    ~TreeSitterParser() = default;

    Ref<ParserError> parse_file(const String& p_path, Ref<Context> r_target);

    Ref<ParserError> parse(const String& p_input, Ref<Context> r_target) override;

    template <typename T>
    static bool register_handler(const String& p_name);

    static String strip_known_macro_contents(const String& p_input, Dictionary<Size, String>& r_parameters);

   private:

    bool input_is_path = false;
    static inline HashSet<String> _registered_handlers;
    static inline Vector<Ref<INodeHandler>> _handlers;
  };

  template <typename T>
  bool TreeSitterParser::register_handler(const String& p_name) {
    if (_registered_handlers.find(p_name) != _registered_handlers.end()) {
      return false;
    }
    _registered_handlers.insert(p_name);
    _handlers.push_back(make_ref<T>());
    return true;
  }

}  // namespace GodotObjectCompiler

#define HANDLER_ERROR(...)                           \
  node_new<ParserError>(ERROR, format(__VA_ARGS__)); \
  return ParserStep::StepOver();

#define HANDLER_ERROR_COND(condition, ...) \
  if ((condition)) {                       \
    HANDLER_ERROR(__VA_ARGS__)             \
  }

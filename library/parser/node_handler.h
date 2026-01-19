#pragma once

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

#include "library/core/core.h"
#include "library/parser/parser.h"
#include "library/tree/all.h"
#include "parser_context.h"

#define NODE_HANDLER(name)                        \
  static inline bool __registered___##name##___ = \
      TreeSitterParser::register_handler<name>();

namespace GodotObjectCompiler {
  enum NextStep { UNDECIDED, STEP_INTO, STEP_OVER, STEP_OUT };

  class INodeHandler {
   public:
    virtual ~INodeHandler() = default;

    virtual bool handles_node(TSNode& node, const String& type) = 0;
    virtual NextStep handle(ParserContext& context) = 0;
  };
}  // namespace GodotObjectCompiler

#define SKIP(name)                                                         \
  class ___##name##___SKIP : public INodeHandler {                         \
    NODE_HANDLER(___##name##___SKIP)                                       \
                                                                           \
   public:                                                                 \
    ~___##name##___SKIP() override = default;                              \
    bool handles_node(TSNode& node, const String& type) override {         \
      return type == #name;                                                \
    }                                                                      \
    NextStep handle(ParserContext& context) override { return STEP_OVER; } \
  }

#define INTO(name)                                                         \
  class ___##name##___SKIP : public INodeHandler {                         \
    NODE_HANDLER(___##name##___SKIP)                                       \
                                                                           \
   public:                                                                 \
    ~___##name##___SKIP() override = default;                              \
    bool handles_node(TSNode& node, const String& type) override {         \
      return type == #name;                                                \
    }                                                                      \
    NextStep handle(ParserContext& context) override { return STEP_INTO; } \
  }

#define SIMPLE_GENERATE(name, type, step)                                \
  class ___##name##___##type##___SIMPLE_GENERATE : public INodeHandler { \
    NODE_HANDLER(___##name##___##type##___SIMPLE_GENERATE)               \
                                                                         \
   public:                                                               \
    ~___##name##___##type##___SIMPLE_GENERATE() override = default;      \
    bool handles_node(TSNode& node, const String& type) override {       \
      return type == #name;                                              \
    }                                                                    \
    NextStep handle(ParserContext& context) override {                   \
      context.current_node->create_child<type>();                        \
      return step;                                                       \
    }                                                                    \
  }

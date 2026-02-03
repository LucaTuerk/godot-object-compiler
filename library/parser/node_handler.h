#pragma once

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

#include "../tree/syntax/all.h"
#include "library/core/core.h"
#include "parser_context.h"
#include "tree_sitter_node.h"
#include "parser.h"

#define NODE_HANDLER(name) static inline bool __registered___##name##___ = TreeSitterParser::register_handler<name>();
#define NODE_HANDLER_V2(name) \
  static inline bool __registered__##name##___ = TreeSitterParser::register_handler_v2<name>(#name)

namespace GodotObjectCompiler {

  enum NextStep { UNDECIDED, STEP_INTO, STEP_OVER, STEP_OUT, STEP_OVER_SPECIFIC };

  class ParserStep {
   public:

    enum Type { TYPE_NONE, TYPE_UNDECIDED, TYPE_STEP_INTO, TYPE_STEP_OVER, TYPE_STEP_OUT, TYPE_GO_TO };

    static ParserStep Undecided();
    static ParserStep StepInto();
    static ParserStep StepOver();
    static ParserStep StepOut();
    static ParserStep GoTo(Ref<TreeSitterNode> target);

    bool is_undecided();
    bool is_step_into();
    bool is_step_over();
    bool is_step_out();
    bool is_go_to(Ref<TreeSitterNode>& target);

   private:

    ParserStep(Type type, const Ref<TreeSitterNode>& goto_target) : type(type), goto_target(goto_target) {}

    Type type = TYPE_NONE;
    Ref<TreeSitterNode> goto_target = nullptr;
  };

  class INodeHandler {
   public:

    virtual ~INodeHandler() = default;

    virtual bool handles_node(TSNode& node, const String& type) = 0;
    virtual NextStep handle(ParserContext& context) = 0;

    static TSNode find_child_of_type(TSNode node, const String& type);
    static TSNode find_ancestor_of_type(TSNode node, const String& type);
  };

  class INodeHandlerV2 {
   public:

    virtual ~INodeHandlerV2() = default;

    virtual bool handles_node(const Ref<TreeSitterNode>& current_src) = 0;
    virtual ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) = 0;
  };

  template <typename T>
  class SkipHandler : public INodeHandlerV2 {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  template <typename T>
  class IntoHandler : public INodeHandlerV2 {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  template <typename T, typename G>
  class GenerateHandler : public INodeHandlerV2 {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  template <typename T>
  bool SkipHandler<T>::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == T::skip_type;
  }

  template <typename T>
  ParserStep SkipHandler<T>::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    return ParserStep::StepOver();
  }

  template <typename T>
  bool IntoHandler<T>::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == T::into_type;
  }

  template <typename T>
  ParserStep IntoHandler<T>::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    return ParserStep::StepInto();
  }

  template <typename T, typename G>
  bool GenerateHandler<T, G>::handles_node(const Ref<TreeSitterNode>& current_src) {
    return current_src->type == T::generate_type;
  }

  template <typename T, typename G>
  ParserStep GenerateHandler<T, G>::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
    current_target->create_child<G>();
    return T::next_step;
  }

}  // namespace GodotObjectCompiler

inline GodotObjectCompiler::ParserStep GodotObjectCompiler::ParserStep::Undecided() {
  return {TYPE_UNDECIDED, nullptr};
}

inline GodotObjectCompiler::ParserStep GodotObjectCompiler::ParserStep::StepInto() { return {TYPE_STEP_INTO, nullptr}; }

inline GodotObjectCompiler::ParserStep GodotObjectCompiler::ParserStep::StepOver() { return {TYPE_STEP_OVER, nullptr}; }

inline GodotObjectCompiler::ParserStep GodotObjectCompiler::ParserStep::StepOut() { return {TYPE_STEP_OUT, nullptr}; }

inline GodotObjectCompiler::ParserStep GodotObjectCompiler::ParserStep::GoTo(Ref<TreeSitterNode> target) {
  return {TYPE_UNDECIDED, target};
}

inline bool GodotObjectCompiler::ParserStep::is_undecided() { return type == TYPE_UNDECIDED; }

inline bool GodotObjectCompiler::ParserStep::is_step_into() { return type == TYPE_STEP_INTO; }

inline bool GodotObjectCompiler::ParserStep::is_step_over() { return type == TYPE_STEP_OVER; }

inline bool GodotObjectCompiler::ParserStep::is_step_out() { return type == TYPE_STEP_OUT; }

inline bool GodotObjectCompiler::ParserStep::is_go_to(Ref<TreeSitterNode>& target) {
  const bool result = type == TYPE_GO_TO;
  if (result) {
    target = goto_target;
  }
  return result;
}

inline TSNode GodotObjectCompiler::INodeHandler::find_child_of_type(TSNode node, const String& type) {
  for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
    TSNode child = ts_node_child(node, i);
    if (String(ts_node_type(child)) == type) {
      return child;
    }
  }
  return TSNode();
}

inline TSNode GodotObjectCompiler::INodeHandler::find_ancestor_of_type(TSNode node, const String& type) {
  TSNode ancestor = node;
  do {
    ancestor = ts_node_parent(ancestor);
  } while (!ts_node_is_null(ancestor) && ts_node_type(ancestor) != type);
  return ancestor;
}

#define SKIP(name)                                                                         \
  class ___##name##___SKIP : public INodeHandler {                                         \
    NODE_HANDLER(___##name##___SKIP)                                                       \
                                                                                           \
   public:                                                                                 \
                                                                                           \
    ~___##name##___SKIP() override = default;                                              \
    bool handles_node(TSNode& node, const String& type) override { return type == #name; } \
    NextStep handle(ParserContext& context) override { return STEP_OVER; }                 \
  }

#define INTO(name)                                                                         \
  class ___##name##___SKIP : public INodeHandler {                                         \
    NODE_HANDLER(___##name##___SKIP)                                                       \
                                                                                           \
   public:                                                                                 \
                                                                                           \
    ~___##name##___SKIP() override = default;                                              \
    bool handles_node(TSNode& node, const String& type) override { return type == #name; } \
    NextStep handle(ParserContext& context) override { return STEP_INTO; }                 \
  }

#define SIMPLE_GENERATE(name, type, step)                                                  \
  class ___##name##___##type##___SIMPLE_GENERATE : public INodeHandler {                   \
    NODE_HANDLER(___##name##___##type##___SIMPLE_GENERATE)                                 \
                                                                                           \
   public:                                                                                 \
                                                                                           \
    ~___##name##___##type##___SIMPLE_GENERATE() override = default;                        \
    bool handles_node(TSNode& node, const String& type) override { return type == #name; } \
    NextStep handle(ParserContext& context) override {                                     \
      context.current_node->create_child<type>();                                          \
      return step;                                                                         \
    }                                                                                      \
  }

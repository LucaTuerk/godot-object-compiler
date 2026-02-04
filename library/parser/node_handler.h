#pragma once

#include "library/core/core.h"
#include "tree_sitter_node.h"

namespace GodotObjectCompiler {

  class ParserStep {
   public:

    enum StepType { TYPE_NONE, TYPE_UNDECIDED, TYPE_STEP_INTO, TYPE_STEP_OVER, TYPE_STEP_OUT, TYPE_GO_TO };

    static ParserStep Undecided();
    static ParserStep StepInto();
    static ParserStep StepOver();
    static ParserStep StepOut();
    static ParserStep GoTo(const Ref<TreeSitterNode>& target);

    [[nodiscard]] bool is_undecided() const;
    [[nodiscard]] bool is_step_into() const;
    [[nodiscard]] bool is_step_over() const;
    [[nodiscard]] bool is_step_out() const;
    bool is_go_to(Ref<TreeSitterNode>& target) const;

   private:

    ParserStep(StepType type, const Ref<TreeSitterNode>& goto_target) : type(type), goto_target(goto_target) {}

    StepType type = TYPE_NONE;
    Ref<TreeSitterNode> goto_target = nullptr;
  };

  class INodeHandler {
   public:

    virtual ~INodeHandler() = default;

    virtual bool handles_node(const Ref<TreeSitterNode>& current_src) = 0;
    virtual ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) = 0;
  };

  template <typename T>
  class SkipHandler : public INodeHandler {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  template <typename T>
  class IntoHandler : public INodeHandler {
   public:

    bool handles_node(const Ref<TreeSitterNode>& current_src) override;
    ParserStep handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) override;
  };

  template <typename T, typename G>
  class GenerateHandler : public INodeHandler {
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

#define NODE_HANDLER(name) \
  static inline bool __registered__##name##___ = TreeSitterParser::register_handler<name>(#name)

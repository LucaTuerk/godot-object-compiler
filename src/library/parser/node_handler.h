/**************************************************************************/
/* node_handler.h                                                         */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#pragma once

#include "library/core/core.h"
#include "tree_sitter_node.h"

namespace GodotObjectCompiler {

class ParserStep {
 public:
  enum StepType {
    TYPE_NONE,
    TYPE_UNDECIDED,
    TYPE_STEP_INTO,
    TYPE_STEP_OVER,
    TYPE_STEP_OUT,
    TYPE_GO_TO
  };

  static ParserStep Undecided();
  static ParserStep StepInto();
  static ParserStep StepOver();
  static ParserStep StepOut();
  static ParserStep GoTo(const Ref<TreeSitterNode>& target);

  [[nodiscard]] bool is_undecided() const;
  [[nodiscard]] bool is_step_into() const;
  [[nodiscard]] bool is_step_over() const;
  [[nodiscard]] bool is_step_out() const;
  bool is_go_to(Ref<TreeSitterNode>& p_target) const;

 private:
  ParserStep(StepType type, const Ref<TreeSitterNode>& goto_target)
      : type(type), goto_target(goto_target) {}

  StepType type = TYPE_NONE;
  Ref<TreeSitterNode> goto_target = nullptr;
};

class INodeHandler {
 public:
  virtual ~INodeHandler() = default;

  virtual String get_type() = 0;
  virtual bool handles_node(
      [[maybe_unused]] const Ref<TreeSitterNode>& p_current_src) = 0;
  virtual ParserStep handle(
      [[maybe_unused]] const Ref<TreeSitterNode>& p_current_src,
      [[maybe_unused]] Ref<Context>& r_current_target) = 0;
};

template <typename T>
class SkipHandler : public INodeHandler {
 public:
  bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
  ParserStep handle(const Ref<TreeSitterNode>& p_current_src,
                    Ref<Context>& r_current_target) override;
};

template <typename T>
class IntoHandler : public INodeHandler {
 public:
  bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
  ParserStep handle(const Ref<TreeSitterNode>& p_current_src,
                    Ref<Context>& r_current_target) override;
};

template <typename T, typename G>
class GenerateHandler : public INodeHandler {
 public:
  bool handles_node(const Ref<TreeSitterNode>& p_current_src) override;
  ParserStep handle(const Ref<TreeSitterNode>& p_current_src,
                    Ref<Context>& r_current_target) override;
};

template <typename T>
bool SkipHandler<T>::handles_node(const Ref<TreeSitterNode>& p_current_src) {
  return p_current_src->type == T::skip_type;
}

template <typename T>
ParserStep SkipHandler<T>::handle(const Ref<TreeSitterNode>& p_current_src,
                                  Ref<Context>& r_current_target) {
  UNUSED(p_current_src);
  UNUSED(r_current_target);

  return ParserStep::StepOver();
}

template <typename T>
bool IntoHandler<T>::handles_node(const Ref<TreeSitterNode>& p_current_src) {
  return p_current_src->type == T::into_type;
}

template <typename T>
ParserStep IntoHandler<T>::handle(const Ref<TreeSitterNode>& p_current_src,
                                  Ref<Context>& r_current_target) {
  UNUSED(p_current_src);
  UNUSED(r_current_target);

  return ParserStep::StepInto();
}

template <typename T, typename G>
bool GenerateHandler<T, G>::handles_node(
    const Ref<TreeSitterNode>& p_current_src) {
  return p_current_src->type == T::generate_type;
}

template <typename T, typename G>
ParserStep GenerateHandler<T, G>::handle(
    const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
  UNUSED(p_current_src);
  r_current_target->create_child<G>();
  return T::next_step;
}

}  // namespace GodotObjectCompiler

#define NODE_HANDLER(name)                       \
 public:                                         \
  String get_type() override { return #name; }   \
                                                 \
 private:                                        \
  static inline bool __registered__##name##___ = \
      TreeSitterParser::register_handler<name>(#name);

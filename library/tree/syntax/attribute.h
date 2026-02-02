#pragma once
#include "context.h"
#include "library/parser/attribute_argument_parser.h"

#define ATTRIBUTE_TYPE(node_type, target_type, target)                                                             \
  NODE_TYPE(node_type)                                                                                             \
 protected:                                                                                                        \
                                                                                                                   \
  virtual bool _verify_target_class(Ref<Node> p_resolved) const override { return p_resolved->is<target_type>(); } \
                                                                                                                   \
  virtual Target _get_target() const override { return target; }                                                   \
                                                                                                                   \
 public:                                                                                                           \
                                                                                                                   \
  Ref<target_type> Target##target_type() {                                                                         \
    Ref<Node> node_target = resolve_target();                                                                      \
    if (!node_target) {                                                                                            \
      return nullptr;                                                                                              \
    }                                                                                                              \
    return node_target->as<target_type>();                                                                         \
  }                                                                                                                \
                                                                                                                   \
 private:

namespace GodotObjectCompiler {

  class Attribute : public NamedContext {
   public:

    enum Target {
      NEXT,        // attribute applies to next sibling in the context
      CONTAINING,  // attribute applies to the containing context
      NONE,
    };

    Ref<Node> resolve_target() const;
    bool verify_target(Ref<Node> p_resolved) const;
    virtual Ref<IAttributeArgumentParser> get_argument_parser();

    Size start;
    Size end;
    Size line;

   protected:

    virtual Target _get_target() const = 0;
    virtual bool _verify_target_class(Ref<Node> p_resolved) const = 0;
    virtual bool _verify_target(Ref<Node> p_resolved) const;
  };

}  // namespace GodotObjectCompiler

#pragma once
#include "context.h"

#define ATTRIBUTE_TYPE(node_type, target_type, target)                                                         \
  NODE_TYPE(node_type)                                                                                         \
 protected:                                                                                                    \
                                                                                                               \
  virtual bool _verify_target_class(Node* p_resolved) const override { return p_resolved->is<target_type>(); } \
                                                                                                               \
  virtual Target _get_target() const override { return target; }                                               \
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

    Node* resolve_target() const;
    bool verify_target(Node* p_resolved) const;

    Size start;
    Size end;
    Size line;

   protected:

    virtual Target _get_target() const = 0;
    virtual bool _verify_target_class(Node* p_resolved) const = 0;
    virtual bool _verify_target(Node* p_resolved) const;
  };

}  // namespace GodotObjectCompiler

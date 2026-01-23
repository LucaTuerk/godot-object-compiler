#pragma once

#include "library/core/core.h"
#include "library/core/reader_writer.h"
#include "library/execution_context.h"
#include "library/node_db.h"

namespace GodotObjectCompiler {
  class Include;
  class Context;
  class Node;

  class Node {
   public:
    virtual String get_type() const { return "Node"; }
    virtual String to_string() const { return get_type(); }
    virtual bool copy_to(Node* other) const { return true; }
    virtual Node* create() { return ExecutionContext::instance()->get_node_db()->create<Node>(); }
    static String get_type_static() { return "Node"; }

    Node() = default;
    virtual ~Node() = default;

    Node* clone();

    virtual void write_to(IStructuredWriter* writer);
    virtual void read_from(IStructuredReader* reader);

    String pretty_print() const;

    UID get_id() const;
    Index get_index() const;
    Index get_depth() const;
    Context* get_parent() const;
    Node* get_root();

    void reparent(Context* new_parent);

    Node* get_sibling(int p_offset) const;
    Node* get_next_sibling() const;
    Node* get_previous_sibling() const;

    template <class T>
    T* find_parent();

    template <class T>
    T* find_previous_sibling();

    template <class T>
    bool is() const;

    template <class T>
    T* as();

    template <class T>
    const T* const_as() const;

   private:
    mutable Node* _root = nullptr;
    Context* _parent = nullptr;
    UID _id = INVALID_ID;
    Index _index = INVALID_INDEX;

    friend class NodeDB;
    friend class Context;
  };

  template <typename T>
  Node* default_construct();

  template <typename T, typename... Args>
  T* node_new(Args&&... args) {
    return ExecutionContext::instance()->get_node_db()->create<T>(std::forward<Args>(args)...);
  }
}  // namespace GodotObjectCompiler

template <class T>
bool GodotObjectCompiler::Node::is() const {
  return dynamic_cast<const T*>(this) != nullptr;
}

template <class T>
T* GodotObjectCompiler::Node::as() {
  return dynamic_cast<T*>(this);
}

template <class T>
const T* GodotObjectCompiler::Node::const_as() const {
  return dynamic_cast<const T*>(this);
}

template <typename T>
GodotObjectCompiler::Node* GodotObjectCompiler::default_construct() {
  return ExecutionContext::instance()->get_node_db()->create<T>();
}

#define NODE_TYPE(type)                                                                                \
 public:                                                                                               \
  type() = default;                                                                                    \
  virtual String get_type() const override { return #type; }                                           \
  static String get_type_static() { return #type; }                                                    \
  static Node* create_static() { return ExecutionContext::instance()->get_node_db()->create<type>(); } \
  virtual Node* create() override { return type::create_static(); }                                    \
  static inline bool __registered__##type##__ =                                                        \
      NodeDB::register_node_constructor(#type, &GodotObjectCompiler::default_construct<type>);

#define COPY_GUARD(type, parent)   \
  auto target = other->as<type>(); \
  if (!target) {                   \
    return false;                  \
  }                                \
  if (!parent::copy_to(target)) {  \
    return false;                  \
  }

#define COPY_LAZY(name) target->_##name##_lazy = name();

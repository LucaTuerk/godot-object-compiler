#pragma once

#include "library/core/core.h"
#include "library/core/reader_writer.h"
#include "library/execution_context.h"
#include "library/node_db.h"

namespace GodotObjectCompiler {

  class Include;
  class Context;
  class Node;

  class Node : public std::enable_shared_from_this<Node> {
   public:

    virtual String get_type() const { return "Node"; }

    virtual String to_string() const { return get_type(); }

    virtual bool copy_to(Ref<Node> other) const { return true; }

    virtual Ref<Node> create() { return ExecutionContext::instance()->get_node_db()->create<Node>(); }

    static String get_type_static() { return "Node"; }

    Node() = default;
    virtual ~Node() = default;

    Ref<Node> clone();

    virtual void write_to(IStructuredWriter* writer);
    virtual void read_from(IStructuredReader* reader);

    String pretty_print() const;

    UID get_id() const;
    Index get_index() const;
    Index get_depth() const;
    Ref<Context> get_parent() const;
    Ref<Node> get_root();

    void reparent(Ref<Context> new_parent);

    Ref<Node> get_sibling(int p_offset) const;
    Ref<Node> get_next_sibling() const;
    Ref<Node> get_previous_sibling() const;

    template <typename T>
    static bool default_node_predicate(Ref<T>) {
      return true;
    }

    template <class T>
    Ref<T> find_parent(Predicate<T> predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_previous_sibling();

    template <class T>
    bool is() const;

    template <class T>
    Ref<T> as();

    template <class T>
    Ref<const T> const_as() const;

   private:

    mutable Ref<Node> _root = nullptr;
    WeakRef<Context> _parent;
    UID _id = INVALID_ID;
    Index _index = INVALID_INDEX;

    friend class NodeDB;
    friend class Context;
  };

  template <typename T>
  Ref<Node> default_construct();

  template <typename T, typename... Args>
  Ref<T> node_new(Args&&... args) {
    return ExecutionContext::instance()->get_node_db()->create<T>(std::forward<Args>(args)...);
  }

}  // namespace GodotObjectCompiler

template <class T>
bool GodotObjectCompiler::Node::is() const {
  return std::dynamic_pointer_cast<const T>(shared_from_this()) != nullptr;
}

template <class T>
Ref<T> GodotObjectCompiler::Node::as() {
  return std::dynamic_pointer_cast<T>(shared_from_this());
}

template <class T>
Ref<const T> GodotObjectCompiler::Node::const_as() const {
  return std::dynamic_pointer_cast<const T>(shared_from_this());
}

template <typename T>
Ref<GodotObjectCompiler::Node> GodotObjectCompiler::default_construct() {
  return ExecutionContext::instance()->get_node_db()->create<T>();
}

#define NODE_TYPE(type)                                                                                \
 public:                                                                                               \
                                                                                                       \
  type() = default;                                                                                    \
  virtual String get_type() const override { return #type; }                                           \
  static String get_type_static() { return #type; }                                                    \
  static Ref<Node> create_static() { return ExecutionContext::instance()->get_node_db()->create<type>(); } \
  virtual Ref<Node> create() override { return type::create_static(); }                                    \
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

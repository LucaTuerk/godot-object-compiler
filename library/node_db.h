#pragma once
#include "core/core.h"

namespace GodotObjectCompiler {

  class Node;
  class ExecutionContext;

  class NodeDB {
   public:

    UID request_id_change(UID from, UID to);

    template <typename T, typename... Args>
    Ref<T> create(Args&&... args);

    template <typename T, typename... Args>
    Ref<T> create_with_id(UID uid, Args&&... args);

    template <typename T>
    Ref<T> get(UID uid);

    template <typename T>
    Vector<Ref<Node>>& get_of_type();

    using NodeCreator = Creator<Node>;

    static bool register_node_constructor(const String& name, NodeCreator);

    static Ref<Node> create(const String& type);

   private:

    NodeDB() = default;
    ~NodeDB();

    static inline HashSet<UID> _uids{};
    static UID _generate_unique_id();
    static bool _has_uid(UID);

    Dictionary<UID, Ref<Node>> _nodes;
    Dictionary<TypeIndex, Vector<Ref<Node>>> _type_nodes;

    static inline Dictionary<String, NodeCreator> _node_constructors;

    friend ExecutionContext;
  };

  template <typename T, typename... Args>
  Ref<T> NodeDB::create(Args&&... args) {
    return create_with_id<T, Args...>(_generate_unique_id(), std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  Ref<T> NodeDB::create_with_id(UID uid, Args&&... args) {
    uid = uid == INVALID_ID ? _generate_unique_id() : uid;

    if (_nodes.find(uid) != _nodes.end()) {
      Ref<Node> other = _nodes[uid];
      auto thing = T::get_type_static();
      print_err("Node with uid already exits");
      return nullptr;
    }

    Ref<T> node = make_ref<T>(std::forward<Args>(args)...);
    node->_id = uid;
    _nodes[uid] = node;
    _type_nodes[typeid(T)].push_back(node);
    return node;
  }

  template <typename T>
  Ref<T> NodeDB::get(UID uid) {
    if (uid == INVALID_ID) return nullptr;

    if (_nodes.find(uid) != _nodes.end()) {
      return std::dynamic_pointer_cast<T>(_nodes[uid]);
    }

    return nullptr;
  }

  template <typename T>
  Vector<Ref<Node>>& NodeDB::get_of_type() {
    return _type_nodes[typeid(T)];
  }

}  // namespace GodotObjectCompiler

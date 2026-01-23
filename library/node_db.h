#pragma once
#include "core/core.h"

namespace GodotObjectCompiler {
  class Node;
  class ExecutionContext;

  class NodeDB {
   public:
    UID request_id_change(UID from, UID to);

    template <typename T, typename... Args>
    T* create(Args&&... args);

    template <typename T, typename... Args>
    T* create_with_id(UID uid, Args&&... args);

    template <typename T>
    T* get(UID uid);

    template <typename T>
    Vector<Node*>& get_of_type();

    using NodeCreator = Creator<Node>;

    static bool register_node_constructor(const String& name, NodeCreator);

    static Node* create(const String& type);

   private:
    NodeDB() = default;
    ~NodeDB();

    static inline HashSet<UID> _uids{};
    static UID _generate_unique_id();
    static bool _has_uid(UID);

    Dictionary<UID, Node*> _nodes;
    Dictionary<TypeIndex, Vector<Node*>> _type_nodes;

    static inline Dictionary<String, NodeCreator> _node_constructors;

    friend ExecutionContext;
  };

  template <typename T, typename... Args>
  T* NodeDB::create(Args&&... args) {
    return create_with_id<T, Args...>(_generate_unique_id(), std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T* NodeDB::create_with_id(UID uid, Args&&... args) {
    uid = uid == INVALID_ID ? _generate_unique_id() : uid;

    if (_nodes.find(uid) != _nodes.end()) {
      Node* other = _nodes[uid];
      auto thing = T::get_type_static();
      print_err("Node with uid already exits");
      return nullptr;
    }

    T* node = new T(std::forward<Args>(args)...);
    node->_id = uid;
    _nodes[uid] = node;
    _type_nodes[typeid(T)].push_back(node);
    return node;
  }

  template <typename T>
  T* NodeDB::get(UID uid) {
    if (uid == INVALID_ID) return nullptr;

    if (_nodes.find(uid) != _nodes.end()) {
      return dynamic_cast<T*>(_nodes[uid]);
    }

    return nullptr;
  }

  template <typename T>
  Vector<Node*>& NodeDB::get_of_type() {
    return _type_nodes[typeid(T)];
  }
}  // namespace GodotObjectCompiler

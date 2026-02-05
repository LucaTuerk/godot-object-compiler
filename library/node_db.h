#pragma once
#include "core/core.h"

namespace GodotObjectCompiler {

  class Node;
  class ExecutionContext;

  class NodeDB {
   public:

    UID request_id_change(UID from, UID to = INVALID_ID);

    template <typename T, typename... Args>
    Ref<T> create(Args&&... args);

    template <typename T, typename... Args>
    Ref<T> create_with_id(UID uid, Args&&... args);

    template <typename T>
    Ref<T> get(UID uid);

    using NodeCreator = Creator<Node>;

    static bool register_node_constructor(const String& name, NodeCreator);

    static Ref<Node> create(const String& type);

   private:

    NodeDB() = default;
    ~NodeDB();

    static inline HashSet<UID> _uids{};
    static UID _generate_unique_id();
    static bool _has_uid(UID);

    Dictionary<UID, WeakRef<Node>> _nodes;
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

    if (auto itr = _nodes.find(uid); itr != _nodes.end()) {
      Ref<Node> other = itr->second.lock();
      if (other != nullptr) {
        print_err("Node with uid already exits");
        return nullptr;
      }
    }

    Ref<T> node = make_ref<T>(std::forward<Args>(args)...);
    node->_id = uid;
    _nodes[uid] = node;
    return node;
  }

  template <typename T>
  Ref<T> NodeDB::get(UID uid) {
    if (uid == INVALID_ID) return nullptr;

    if (auto itr = _nodes.find(uid); itr != _nodes.end()) {
      return std::dynamic_pointer_cast<T>(itr->second.lock());
    }

    return nullptr;
  }

}  // namespace GodotObjectCompiler

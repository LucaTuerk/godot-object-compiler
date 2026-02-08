#pragma once
#include "core/core.h"

namespace GodotObjectCompiler {

  class Node;
  class ExecutionContext;

  class NodeDB {
   public:

    UID request_id_change(UID p_from, UID p_to = INVALID_ID);

    template <typename T, typename... Args>
    Ref<T> create(Args&&... p_args);

    template <typename T, typename... Args>
    Ref<T> create_with_id(UID p_uid, Args&&... p_args);

    template <typename T>
    Ref<T> get(UID p_uid);

    using NodeCreator = Creator<Node>;

    static bool register_node_constructor(const String& p_name, NodeCreator p_creator);

    static Ref<Node> create(const String& p_type);

   private:

    NodeDB() = default;
    ~NodeDB();

    static inline HashSet<UID> _uids{};

    static UID _generate_unique_id();

    static bool _has_uid(UID p_uid);

    Dictionary<UID, WeakRef<Node>> _nodes;
    static inline Dictionary<String, NodeCreator> _node_constructors;

    friend ExecutionContext;
  };

  template <typename T, typename... Args>
  Ref<T> NodeDB::create(Args&&... p_args) {
    return create_with_id<T, Args...>(_generate_unique_id(), std::forward<Args>(p_args)...);
  }

  template <typename T, typename... Args>
  Ref<T> NodeDB::create_with_id(UID p_uid, Args&&... p_args) {
    p_uid = p_uid == INVALID_ID ? _generate_unique_id() : p_uid;

    if (const auto itr = _nodes.find(p_uid); itr != _nodes.end()) {
      if (const Ref<Node> other = itr->second.lock(); other != nullptr) {
        print_err("Node with uid already exits");
        return nullptr;
      }
    }

    Ref<T> node = make_ref<T>(std::forward<Args>(p_args)...);
    node->_id = p_uid;
    _nodes[p_uid] = node;
    return node;
  }

  template <typename T>
  Ref<T> NodeDB::get(UID p_uid) {
    if (p_uid == INVALID_ID) return nullptr;

    if (const auto itr = _nodes.find(p_uid); itr != _nodes.end()) {
      return std::dynamic_pointer_cast<T>(itr->second.lock());
    }

    return nullptr;
  }

}  // namespace GodotObjectCompiler

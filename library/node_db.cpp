
#include "node_db.h"

#include <random>

#include "tree/syntax/node.h"

namespace GodotObjectCompiler {

  UID NodeDB::request_id_change(UID p_from, UID p_to) {
    if (p_from == INVALID_ID) {
      print_err("Trying to change a node id from invalid id. This is not possible.");
      return p_from;
    }

    if (p_to == INVALID_ID) {
      p_to = _generate_unique_id();
    }

    if (_nodes.find(p_from) == _nodes.end()) {
      print_err("Trying to change a node id, but no node is registered for this id.");
      return p_from;
    }

    _uids.erase(p_from);
    _uids.insert(p_to);
    _nodes[p_to] = _nodes[p_from];
    _nodes.erase(p_from);
    return p_to;
  }

  NodeDB::~NodeDB() {}

  Ref<Node> NodeDB::create(const String& p_type) {
    auto itr = _node_constructors.find(p_type);

    if (itr == _node_constructors.end()) {
      return nullptr;
    }

    return itr->second();
  }

  bool NodeDB::register_node_constructor(const String& p_name, NodeCreator p_creator) {
    return _node_constructors.insert({p_name, p_creator}).second;
  }

  UID NodeDB::_generate_unique_id() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    UID generated = dis(gen);
    while (_has_uid(generated)) {
      generated = dis(gen);
    }

    _uids.insert(generated);
    return generated;
  }

  bool NodeDB::_has_uid(UID p_uid) { return _uids.find(p_uid) != _uids.end(); }

}  // namespace GodotObjectCompiler

#include "node_db.h"

#include <random>

#include "tree/syntax/node.h"

namespace GodotObjectCompiler {

  UID NodeDB::request_id_change(UID from, UID to) {
    if (from == INVALID_ID) {
      print_err("Trying to change a node id from invalid id. This is not possible.");
      return from;
    }

    if (_nodes.find(from) == _nodes.end()) {
      print_err("Trying to change a node id, but no node is registered for this id.");
      return from;
    }

    _uids.erase(from);
    _uids.insert(to);
    _nodes[to] = _nodes[from];
    _nodes.erase(from);
    return to;
  }

  NodeDB::~NodeDB() {
  }

  Ref<Node> NodeDB::create(const String& type) {
    auto itr = _node_constructors.find(type);

    if (itr == _node_constructors.end()) {
      return nullptr;
    }

    return itr->second();
  }

  bool NodeDB::register_node_constructor(const String& name, NodeCreator constructor) {
    return _node_constructors.insert({name, constructor}).second;
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

  bool NodeDB::_has_uid(UID uid) { return _uids.find(uid) != _uids.end(); }

}  // namespace GodotObjectCompiler
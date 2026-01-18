#include "node.h"
#include "library/core/config.h"
#include "library/core/core.h"

namespace GodotObjectCompiler {

    NodeDB::~NodeDB() {
        for (auto &[uid, node] : _nodes) {
            // clean up unreferenced nodes
            if(!node->_parent) {
                delete node;
            }
        }
    }

    UID NodeDB::_generate_unique_id() {
        return rand();
    }

    UID Node::get_id() const {
        return _id;
    }

    Context* Node::get_parent() const {
        return _parent;
    }

    void Node::dump_properties(Config& config) {
        config.set("_class", get_type());
        config.set("_id", get_id());
        config.set("_parent", _parent ? _parent->get_id() : INVALID_ID);
    }

    Context::~Context() {
        for (auto child : _children) {
            delete child;
        }
    }

    void Context::add_child(Node* p_child) {
        p_child->_parent = this;
        _children.push_back(p_child);
    }

    Size Context::get_child_count() const {
        return _children.size();
    }

    Node* Context::get_child(Size p_idx) const {
        return _children[p_idx];
    }

    void Context::dump_properties(Config& config) {
        Node::dump_properties(config);
    }
}

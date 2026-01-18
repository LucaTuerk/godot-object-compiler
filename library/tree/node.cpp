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

    void Node::write_to(IWriter *writer) {
        writer->write("_class", get_type());
        writer->write("_id", get_id());
        writer->write("_parent", _parent ? _parent->get_id() : INVALID_ID);
    }

	void Node::read_from(IReader *reader) {
    	_id = reader->read<String, UID>("_id");
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

    void Context::write_to(IWriter *writer) {
        Node::write_to(writer);
    }
}

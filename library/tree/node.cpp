#include "node.h"
#include "../core/io/config.h"
#include "library/core/core.h"

#include <strings.h>

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

	Index Node::get_index() const {
    	return _index;
	}

	Context * Node::get_parent() const {
        return _parent;
    }

	Node *Node::get_sibling(int p_offset) const {
		if (_parent == nullptr) {
			return nullptr;
		}

		return _parent->get_child(_index + p_offset);
    }

	Node *Node::get_next_sibling() const {
    	return get_sibling(+1);
	}

	Node *Node::get_previous_sibling() const {
    	return get_sibling(-1);
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
    	p_child->_index = _children.size();
        _children.push_back(p_child);
    }

    Size Context::get_child_count() const {
        return _children.size();
    }

    Node* Context::get_child(Index p_idx) const {
    	if (p_idx >= _children.size()) {
    		return nullptr;
    	}

        return _children[p_idx];
    }

    void Context::write_to(IWriter *writer) {
        Node::write_to(writer);
    }
}

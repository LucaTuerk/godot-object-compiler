#include "node.h"

#include <strings.h>

#include "../core/io/config.h"
#include "identifier.h"
#include "library/core/core.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  NodeDB::~NodeDB() {
    for (auto& [uid, node] : _nodes) {
      // clean up unreferenced nodes
      if (!node->_parent) {
        delete node;
      }
    }
  }

  UID NodeDB::_generate_unique_id() { return rand(); }

  UID Node::get_id() const { return _id; }

  Index Node::get_index() const { return _index; }

  Index Node::get_depth() const {
    Index depth = 0;
    Context* current = _parent;

    while (current) {
      depth++;
      current = current->_parent;
    }

    return depth;
  }

  Context* Node::get_parent() const { return _parent; }

  void Node::reparent(Context* new_parent) {
    if (_parent) {
      _parent->remove_child(this);
    }
    new_parent->add_child(this);
  }

  Node* Node::get_sibling(int p_offset) const {
    if (_parent == nullptr) {
      return nullptr;
    }

    return _parent->get_child_strict((SignedIndex)_index + p_offset);
  }

  Node* Node::get_next_sibling() const { return get_sibling(+1); }

  Node* Node::get_previous_sibling() const { return get_sibling(-1); }

  Node* Node::clone() {
    Node* new_node = create();
    if (copy_to(new_node)) {
      return new_node;
    } else {
      return nullptr;
    }
  }

  void Node::write_to(IWriter* writer) {
    writer->write("_class", get_type());
    writer->write("_id", get_id());
    writer->write("_parent", _parent ? _parent->get_id() : INVALID_ID);
  }

  void Node::read_from(IReader* reader) {
    _id = reader->read<String, UID>("_id");
  }

  String Node::pretty_print() const {
    String result = "";

    Index depth = get_depth();
    for (Index i = 0; i < depth; i++) {
      if (i == depth - 1) {
        result += "    |- ";
      } else {
        result += "   ";
      }
    }

    result += to_string() + "\n";

    if (const auto context = dynamic_cast<const Context*>(this)) {
      for (Index i = 0; i < context->get_child_count(); i++) {
        result += context->get_child(i)->pretty_print();
      }
    }

    return result;
  }

  Context::~Context() {
    for (auto child : _children) {
      delete child;
    }
  }

  bool Context::copy_to(Node* other) const {
    COPY_GUARD(Context, Node);

    for (Node* child : _children) {
      Node* cloned = child->clone();
      if (!cloned) {
        return false;
      }
      target->add_child(cloned);
    }

    return true;
  }

  void Context::add_child(Node* p_child) {
    p_child->_parent = this;
    p_child->_index = _children.size();
    _children.push_back(p_child);
  }

  Size Context::get_child_count() const { return _children.size(); }

  Node* Context::get_child(SignedIndex p_idx) const {
    Index actual_idx = p_idx;
    if (p_idx < 0) {
      actual_idx = _children.size() + p_idx;
    }

    if (actual_idx >= _children.size()) {
      return nullptr;
    }

    return _children[actual_idx];
  }

  Node* Context::get_child_strict(SignedIndex p_idx) const {
    if (p_idx < 0 || static_cast<Size>(p_idx) >= _children.size()) {
      return nullptr;
    }

    return get_child(p_idx);
  }

  Vector<Node*>& Context::get_children() { return _children; }

  void Context::remove_child(Node* p_child) {
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    if (itr != _children.end()) {
      itr = _children.erase(itr);

      while (itr != _children.end()) {
        (*itr)->_index -= 1;
        ++itr;
      }
    }

    Index idx = 0;
    for (auto child : _children) {
      if (idx++ != child->_index) {
        print_err("OHNO");
      }
    }
  }
  void Context::replace_child(Node* p_child, Node* p_new_child) {
    Index index = p_child->_index;
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    if (itr != _children.end()) {
      *itr = p_new_child;
      p_new_child->_index = index;
      p_new_child->_parent = this;
      p_child->_parent = nullptr;
    }
  }

  void Context::write_to(IWriter* writer) { Node::write_to(writer); }

  String NamedContext::_name_lazy_get() {
    Identifier* identifier = find_child<Identifier>();
    if (!identifier) {
      return String();
    }
    return identifier->name;
  }

  String NamedContext::_qualified_name_lazy_get() {
    String result = "";

    Namespace* ns = find_ancestor<Namespace>();
    if (ns) {
      result += ns->qualified_name();
      result += "::";
    }

    return result + name();
  }

  bool NamedContext::copy_to(Node* other) const {
    COPY_GUARD(NamedContext, Context);
    COPY_LAZY(name);
    COPY_LAZY(qualified_name);
    return true;
  }

}  // namespace GodotObjectCompiler

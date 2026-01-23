#include "node.h"

#include "identifier.h"
#include "include.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  UID Node::get_id() const { return _id; }

  Index Node::get_index() const { return _index; }

  Index Node::get_depth() const {
    const Node* current = this;
    Size i = 0;

    do {
      current = current->get_parent();
      if (i++ > 16) {
        return 16;
      }
    } while (current);

    return i;
  }

  Context* Node::get_parent() const { return _parent; }

  Node* Node::get_root() {
    if (!_root) {
      Node* current = this;
      while (current) {
        _root = current;
        current = current->get_parent();
      }
    }

    return _root ? _root : this;
  }

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

  void Node::write_to(IStructuredWriter* writer) {
    writer->write("_class", get_type());
    writer->write("_id", get_id());
    writer->write("_parent", _parent ? _parent->get_id() : INVALID_ID);
  }

  void Node::read_from(IStructuredReader* reader) {
    UID before = _id;
    UID new_id = reader->read<String, UID>("_id");
    if (before != new_id) {
      _id = ExecutionContext::instance()->get_node_db()->request_id_change(before, new_id);
    }
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
        auto child = context->get_child(i);
        auto include = child->as<Include>();
        if (include) {
          // include->evaluate();
        }
        result += child->pretty_print();
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
    if (p_child == this) {
      return;
    }

    if (!p_child) {
      return;
    }

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

    auto itr = _children.begin();
    std::advance(itr, actual_idx);
    return *itr;
  }

  Node* Context::get_child_strict(SignedIndex p_idx) const {
    if (p_idx < 0 || static_cast<Size>(p_idx) >= _children.size()) {
      return nullptr;
    }

    return get_child(p_idx);
  }

  List<Node*>& Context::get_children() { return _children; }
  Context::ChildIterator Context::begin() { return _children.begin(); }
  Context::ChildIterator Context::end() { return _children.end(); }

  void Context::merge_includes(Namespace* target, Size depth) {
    if (!_parent) {
      ChildIterator _ = merge_includes(target, {}, depth);
    }
  }
  Context::ChildIterator Context::merge_includes(Namespace* target, ChildIterator this_itr, Size depth) {
    const Namespace* this_namespace = this->as<Namespace>();
    const Include* this_include = this->as<Include>();

    if (this_include && depth != 0) {
      Include::Error _ = this_include->evaluate();
    }

    if ((this_namespace != nullptr && this_namespace->qualified_name().empty()) || this_include != nullptr) {
      for (auto itr = _children.begin(); itr != _children.end();) {
        if (Context* child_ctx = (*itr)->as<Context>()) {
          itr = child_ctx->merge_includes(target, itr, depth == 0 ? 0 : depth - 1);
        } else {
          itr = reparent_child(itr, target);
        }
      }
    }

    if (_parent) {
      return _parent->reparent_child(this_itr, target);
    }

    return {};
  }

  Context::ChildIterator Context::remove_child(decltype(_children)::iterator itr) {
    Node* child = *itr;
    itr = _children.erase(itr);
    child->_index = 0;
    child->_parent = nullptr;

    auto next = itr;
    while (next != _children.end()) {
      (*next)->_index -= 1;
      ++next;
    }

    return itr;
  }

  Context::ChildIterator Context::reparent_child(decltype(_children)::iterator itr, Context* new_parent) {
    if (itr != _children.end()) {
      Node* child = *itr;
      itr = remove_child(itr);
      new_parent->add_child(child);
      return itr;
    }

    return _children.end();
  }

  void Context::remove_child(Node* p_child) {
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    remove_child(itr);
  }

  void Context::replace_child(Node* p_child, Node* p_new_child, bool p_take_children) {
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    if (itr != _children.end()) {
      *itr = p_new_child;

      p_new_child->_index = p_child->_index;
      p_new_child->_parent = this;

      p_child->_parent = nullptr;

      Context* child_context = p_child->as<Context>();
      Context* new_child_context = p_new_child->as<Context>();

      if (p_take_children && child_context && new_child_context) {
        for (auto itr = child_context->_children.begin(); itr != child_context->_children.end();) {
          itr = child_context->reparent_child(itr, new_child_context);
        }
      }
    }
  }

  void Context::write_to(IStructuredWriter* writer) { Node::write_to(writer); }

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
      auto parent_qualified_name = ns->qualified_name();
      if (!parent_qualified_name.empty()) {
        result += parent_qualified_name;
        result += "::";
      }
    }

    return result + name();
  }

  bool NamedContext::copy_to(Node* other) const {
    COPY_GUARD(NamedContext, Context);
    // COPY_LAZY(name);
    // COPY_LAZY(qualified_name);
    return true;
  }

}  // namespace GodotObjectCompiler

#include "context.h"

#include "identifier.h"
#include "include.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  Context::~Context() {}

  bool Context::copy_to(Ref<Node> other) const {
    COPY_GUARD(Context, Node);

    for (Ref<Node> child : _children) {
      Ref<Node> cloned = child->clone();
      if (!cloned) {
        return false;
      }
      target->add_child(cloned);
    }

    return true;
  }

  void Context::add_child(Ref<Node> p_child) {
    if (!p_child) {
      return;
    }

    if (this == p_child->get_parent().get()) {
      return;
    }

    if (p_child->get_parent() != nullptr) {
      return p_child->reparent(this->as<Context>());
    }

    if (p_child.get() == this) {
      return;
    }

    p_child->_parent = this->as<Context>();
    p_child->_index = _children.size();
    _children.push_back(p_child);
  }

  Size Context::get_child_count() const { return _children.size(); }

  Ref<Node> Context::get_child(SignedIndex p_idx) const {
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

  Ref<Node> Context::get_child_strict(SignedIndex p_idx) const {
    if (p_idx < 0 || static_cast<Size>(p_idx) >= _children.size()) {
      return nullptr;
    }

    return get_child(p_idx);
  }

  List<Ref<Node>>& Context::get_children() { return _children; }

  Context::ChildIterator Context::begin() { return _children.begin(); }

  Context::ChildIterator Context::end() { return _children.end(); }

  void Context::merge_includes(Ref<Namespace> target, Size depth) {
    Ref<Context> parent = get_parent();
    if (!parent) {
      ChildIterator _ = merge_includes(target, {}, depth);
    }
  }

  Context::ChildIterator Context::merge_includes(Ref<Namespace> target, ChildIterator this_itr, Size depth) {
    const Ref<Namespace> this_namespace = this->as<Namespace>();
    const Ref<Include> this_include = this->as<Include>();

    if (this_include && depth != 0) {
      Include::Error _ = this_include->evaluate();
    }

    if ((this_namespace != nullptr && this_namespace->qualified_name().empty()) || this_include != nullptr) {
      for (auto itr = _children.begin(); itr != _children.end();) {
        if (Ref<Context> child_ctx = (*itr)->as<Context>()) {
          itr = child_ctx->merge_includes(target, itr, depth == 0 ? 0 : depth - 1);
        } else {
          itr = reparent_child(itr, target);
        }
      }
    }

    Ref<Context> parent = get_parent();
    if (parent) {
      return parent->reparent_child(this_itr, target);
    }

    return {};
  }

  Context::ChildIterator Context::remove_child(decltype(_children)::iterator itr) {
    Ref<Node> child = *itr;
    itr = _children.erase(itr);
    child->_index = 0;
    child->_parent = {};

    auto next = itr;
    while (next != _children.end()) {
      (*next)->_index -= 1;
      ++next;
    }

    return itr;
  }

  Context::ChildIterator Context::reparent_child(decltype(_children)::iterator itr, Ref<Context> new_parent) {
    if (itr != _children.end()) {
      Ref<Node> child = *itr;
      itr = remove_child(itr);
      new_parent->add_child(child);
      return itr;
    }

    return _children.end();
  }

  void Context::remove_all_children() {
    while (!_children.empty()) {
      Ref<Node> child = _children.back();
      _children.pop_back();
      child->_parent = {};
    }
  }

  void Context::remove_child(Ref<Node> p_child) {
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    remove_child(itr);
  }

  void Context::replace_child(Ref<Node> p_child, Ref<Node> p_new_child, bool p_take_children) {
    auto itr = std::find(_children.begin(), _children.end(), p_child);
    if (itr != _children.end()) {
      *itr = p_new_child;

      p_new_child->_index = p_child->_index;
      p_new_child->_parent = this->as<Context>();

      p_child->_parent = {};

      Ref<Context> child_context = p_child->as<Context>();
      Ref<Context> new_child_context = p_new_child->as<Context>();

      if (p_take_children && child_context && new_child_context) {
        for (auto itr = child_context->_children.begin(); itr != child_context->_children.end();) {
          itr = child_context->reparent_child(itr, new_child_context);
        }
      }
    }
  }

  void Context::write_to(IStructuredWriter* writer) { Node::write_to(writer); }

  String NamedContext::_name_lazy_get() {
    Ref<Identifier> identifier = find_child<Identifier>();
    if (!identifier) {
      return String();
    }
    return identifier->name;
  }

  Ref<Body> NamedContext::_body_lazy_get() { return find_child<Body>(); }

  String NamedContext::_qualified_name_lazy_get() {
    String result = "";

    Ref<Namespace> ns = find_ancestor<Namespace>();
    if (ns) {
      auto parent_qualified_name = ns->qualified_name();
      if (!parent_qualified_name.empty()) {
        result += parent_qualified_name;
        result += "::";
      }
    }

    return result + name();
  }

  bool NamedContext::copy_to(Ref<Node> other) const {
    COPY_GUARD(NamedContext, Context);
    // COPY_LAZY(name);
    // COPY_LAZY(qualified_name);
    return true;
  }

}  // namespace GodotObjectCompiler
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
    Ref<const Node> current = shared_from_this();
    Size i = 0;

    do {
      current = current->get_parent();
      if (i++ > 16) {
        return 16;
      }
    } while (current);

    return i;
  }

  Ref<Context> Node::get_parent() const { return _parent.lock(); }

  Ref<Node> Node::get_root() {
    if (!_root) {
      Ref<Node> current = shared_from_this();
      while (current) {
        _root = current;
        current = current->get_parent();
      }
    }

    return _root ? _root : shared_from_this();
  }

  void Node::reparent(Ref<Context> new_parent) {
    Ref<Context> parent = get_parent();
    if (parent) {
      parent->remove_child(shared_from_this());
    }
    new_parent->add_child(shared_from_this());
  }

  Ref<Node> Node::get_sibling(int p_offset) const {
    Ref<Context> parent = get_parent();
    if (parent == nullptr) {
      return nullptr;
    }

    return parent->get_child_strict((SignedIndex)_index + p_offset);
  }

  Ref<Node> Node::get_next_sibling() const { return get_sibling(+1); }

  Ref<Node> Node::get_previous_sibling() const { return get_sibling(-1); }

  Ref<Node> Node::clone() {
    Ref<Node> new_node = create();
    if (copy_to(new_node)) {
      return new_node;
    } else {
      return nullptr;
    }
  }

  void Node::write_to(IStructuredWriter* writer) {
    Ref<Context> parent = get_parent();
    writer->write("_class", get_type());
    writer->write("_id", get_id());
    writer->write("_parent", parent ? parent->get_id() : INVALID_ID);
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

    if (const auto context = std::dynamic_pointer_cast<const Context>(shared_from_this())) {
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

}  // namespace GodotObjectCompiler

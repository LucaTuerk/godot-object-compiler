#pragma once
#include "identifier.h"
#include "library/core/lazy.h"
#include "node.h"

namespace GodotObjectCompiler {

  class Namespace;
  class Body;

  enum StemExplorationType {
    DIRECT_PARENTS,
    BY_SIBLINGS_PREV,
    BY_SIBLINGS_NEXT,
  };

  enum BranchExplorationType {
    BFS,
    DFS,
  };

  template <typename T, typename... Args>
  class Builder {
   public:

    Builder(Args... args);
    operator Ref<T>();

    operator Ref<Node>();

    template <typename B, typename... BArgs>
    Builder& with_child(BArgs... child_args);

    template <typename B, typename... BArgs>
    Builder& with_child_ref(B** ptr, BArgs... child_args);

    Builder& with_child(Ref<Node> child);
    Builder& with_child_ref(Ref<Node>* ptr, Ref<Node> child);

    Builder& with_children(std::initializer_list<Ref<Node>>&& children);

   private:

    Builder(Ref<Context> parent, Args... args);

    Ref<T> created;

    friend Context;
  };

  class Context : public Node {
    NODE_TYPE(Context)

    ~Context() override;
    bool copy_to(Ref<Node> other) const override;

    void add_child(Ref<Node> p_child);
    void remove_child(Ref<Node> p_child);

    void replace_child(Ref<Node> p_child, Ref<Node> p_new_child, bool take_children = false);

    Size get_child_count() const;
    Ref<Node> get_child(SignedIndex p_idx) const;
    Ref<Node> get_child_strict(SignedIndex p_idx) const;
    List<Ref<Node>>& get_children();

    template <typename T>
    using NodeFunctor = void (*)(Ref<T>);

    List<Ref<Node>> _children{};

    using ChildIterator = decltype(_children)::iterator;

    ChildIterator begin();
    ChildIterator end();

    void merge_includes(Ref<Namespace> target, Size depth = Limits<Size>::max());
    ChildIterator merge_includes(Ref<Namespace> target, ChildIterator this_itr, Size depth);
    ChildIterator remove_child(ChildIterator itr);
    ChildIterator reparent_child(ChildIterator, Ref<Context> new_parent);

    template <typename T>
    Ref<T> find_child(Index p_start_idx = 0, Predicate<T> predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_ancestor(StemExplorationType type = DIRECT_PARENTS, Predicate<T> predicate = default_node_predicate<T>);

    template <class T>
    Ref<T> find_descendant(BranchExplorationType order = BFS, Predicate<T> predicate = default_node_predicate<T>);

    template <class T, typename... Args>
    Ref<T> create_child(Args&&... args);

    template <class T, typename... Args>
    Builder<T, Args...> build_child(Args&&... args);

    template <class T>
    void for_descendants(BranchExplorationType order = BFS, NodeFunctor<T> functor = default_node_predicate<T>);

    void write_to(IStructuredWriter* writer) override;
  };

  class Body : public Context {
    NODE_TYPE(Body);
  };

  class NamedContext : public Context {
    NODE_TYPE(NamedContext)
    bool copy_to(Ref<Node> other) const override;

   private:

    LAZY(NamedContext, Ref<Body>, body);
    LAZY(NamedContext, String, name);
    LAZY(NamedContext, String, qualified_name);
  };

  template <class T>
  Ref<T> Node::find_parent(Predicate<T> predicate) const {
    Ref<Node> current = get_parent();

    while (current) {
      Ref<T> casted = std::dynamic_pointer_cast<T>(current);
      if (casted != nullptr && predicate(casted)) {
        return casted;
      }
      current = current->get_parent();
    }

    return nullptr;
  }

  template <class T>
  Ref<T> Node::find_previous_sibling() {
    Ref<Node> current = get_previous_sibling();

    while (current) {
      if (Ref<T> casted = std::dynamic_pointer_cast<T>(current)) {
        return casted;
      }
      current = current->get_previous_sibling();
    }

    return nullptr;
  }

  template <class T>
  std::shared_ptr<T> Context::find_ancestor(StemExplorationType type, Predicate<T> predicate) {
    Ref<Node> current = get_parent();

    while (current) {
      Ref<T> current_t = current->as<T>();
      if (current_t && predicate(current_t)) {
        return current_t;
      }

      switch (type) {
        {
          case DIRECT_PARENTS:
            current = current->get_parent();
        }
        break;
        case BY_SIBLINGS_PREV: {
          Ref<Node> prev = current->get_previous_sibling();
          current = prev ? prev : current->get_parent();
        } break;
        case BY_SIBLINGS_NEXT: {
          Ref<Node> next = current->get_next_sibling();
          current = next ? next : current->get_parent();
        } break;
      }
    }

    return nullptr;
  }

  template <typename T>
  std::shared_ptr<T> Context::find_child(Index p_start_idx, Predicate<T> predicate) const {
    for (Ref<Node> child : _children) {
      Ref<T> tChild = std::dynamic_pointer_cast<T>(child);
      if (tChild && predicate(tChild)) {
        return tChild;
      }
    }

    return nullptr;
  }

  template <class T>
  std::shared_ptr<T> Context::find_descendant(BranchExplorationType order, Predicate<T> predicate) {
    switch (order) {
      case DFS:
        for (auto child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t && predicate(child_t)) {
            return child_t;
          } else if (child->is<Context>()) {
            Ref<T> child_res = child->as<Context>()->find_descendant<T>(order, predicate);
            if (child_res && predicate(child_res)) {
              return child_res;
            }
          }
        }
        break;
      case BFS:
        for (auto child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t && predicate(child_t)) {
            return child_t;
          }
        }
        for (auto child : _children) {
          if (child->is<Context>()) {
            Ref<T> child_res = child->as<Context>()->find_descendant<T>(order, predicate);
            if (child_res) {
              return child_res;
            }
          }
        }
        break;
    }
    return nullptr;
  }

  template <class T, typename... Args>
  std::shared_ptr<T> Context::create_child(Args&&... args) {
    Ref<T> child = ExecutionContext::instance()->get_node_db()->create<T>(std::forward<Args>(args)...);
    add_child(child);
    return child;
  }

  template <class T, typename... Args>
  Builder<T, Args...> Context::build_child(Args&&... args) {
    return Builder<T, Args...>(this->as<Context>(), std::forward<Args>(args)...);
  }

  template <class T>
  void Context::for_descendants(BranchExplorationType order, NodeFunctor<T> functor) {
    switch (order) {
      case DFS:
        for (auto child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t) {
            functor(child_t);
          } else if (child->is<Context>()) {
            child->as<Context>()->for_descendants<T>(order, functor);
          }
        }
        break;
      case BFS:
        for (auto child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t) {
            functor(child_t);
          }
        }
        for (auto child : _children) {
          if (child->is<Context>()) {
            child->as<Context>()->for_descendants<T>(order, functor);
          }
        }
        break;
    }
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::Builder(Args... args) {
    created = node_new<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::Builder(Ref<Context> parent, Args... args) {
    created = node_new<T>(std::forward<Args>(args)...);
    if (parent) {
      parent->add_child(created);
    }
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::operator Ref<T>() {
    return created;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::operator std::shared_ptr<Node>() {
    return std::dynamic_pointer_cast<Node>(created);
  }

  template <typename T, typename... Args>
  template <typename B, typename... BArgs>
  Builder<T, Args...>& Builder<T, Args...>::with_child(BArgs... child_args) {
    created->add_child(node_new<B>(std::forward<BArgs>(child_args)...));
    return *this;
  }

  template <typename T, typename... Args>
  template <typename B, typename... BArgs>
  Builder<T, Args...>& Builder<T, Args...>::with_child_ref(B** ptr, BArgs... child_args) {
    Ref<B> child = node_new<B>(std::forward<BArgs>(child_args)...);
    created->add_child(child);
    if (ptr) {
      *ptr = child;
    }
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_child_ref(Ref<Node>* ptr, Ref<Node> child) {
    created->add_child(child);
    if (ptr) {
      *ptr = child;
    }
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_child(Ref<Node> child) {
    created->add_child(child);
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_children(std::initializer_list<Ref<Node>>&& children) {
    for (Ref<Node> child : children) {
      created->add_child(child);
    }
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...> build(Args... args) {
    return Builder<T, Args...>(args...);
  }

  template <typename T, typename... Args>
  Builder<T, Args...> build_ref(Ref<T>* ptr, Args... args) {
    Builder<T, Args...> builder(args...);
    *ptr = builder;
    return builder;
  }

}  // namespace GodotObjectCompiler

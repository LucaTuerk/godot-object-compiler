/**************************************************************************/
/* context.h                                                              */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#pragma once
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

    template <typename B, typename = std::enable_if_t<std::is_base_of_v<B, T>>>
    operator Ref<B>();

    operator Ref<Node>();

    template <typename B, typename... BArgs>
    Builder& with_child(BArgs... child_args);

    template <typename B, typename... BArgs>
    Builder& with_child_ref(Ref<B>* ptr, BArgs... child_args);

    Builder& with_child(Ref<Node> p_child);

    Builder& with_child_ref(Ref<Node>* p_node, Ref<Node> p_child);

    Builder& with_children(std::initializer_list<Ref<Node>>&& p_children);

   private:

    Builder(Ref<Context> parent, Args... args);

    Ref<T> _created;

    friend Context;
  };

  class Context : public Node {
    NODE_TYPE(Context)

    bool copy_to(const Ref<Node>& p_other) const override;

    void add_child(Ref<Node> p_child);

    void add_children(std::initializer_list<Ref<Node>>&& p_children);

    void remove_child(const Ref<Node>& p_child);

    void replace_child(const Ref<Node>& p_child, const Ref<Node>& p_new_child, bool take_children = false);

    bool empty() const;

    Size get_child_count() const;

    Size get_descendant_count() const;

    template <typename T>
    Ref<T> get_child(SignedIndex p_idx) const;

    Ref<Node> get_child(SignedIndex p_idx) const;

    Ref<Node> get_child_strict(SignedIndex p_idx) const;

    List<Ref<Node>>& get_children();

    template <typename T>
    using NodeFunctor = void (*)(Ref<T>);

    List<Ref<Node>> _children{};

    using ChildIterator = decltype(_children)::iterator;

    ChildIterator begin();
    ChildIterator end();

    ChildIterator remove_child(ChildIterator p_itr);

    ChildIterator reparent_child(ChildIterator p_itr, Ref<Context> p_new_parent);

    void remove_all_children();

    template <typename T>
    Ref<T> find_chain(Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <typename T, typename C, typename... Args>
    Ref<T> find_chain(Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <typename T>
    Ref<T> find_child(Index p_start_idx = 0, Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_ancestor(
        StemExplorationType p_type = DIRECT_PARENTS, Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_descendant(
        BranchExplorationType p_order = BFS, Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_previous_sibling(Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <class T, typename... Args>
    Ref<T> create_child(Args&&... args);

    template <class T, typename... Args>
    Builder<T, Args...> build_child(Args&&... args);

    template <class T>
    Vector<Ref<T>> find_children(bool p_recursive = false, Predicate<T> p_predicate = default_node_predicate<T>) const;

    void write_to(IStructuredWriter* p_writer) override;
  };

  class Body : public Context {
    NODE_TYPE(Body);
  };

  class NamedContext : public Context {
    NODE_TYPE(NamedContext)

    bool copy_to(const Ref<Node>& p_other) const override;

    void read_from(IStructuredReader* p_reader) override;

    void write_to(IStructuredWriter* p_writer) override;

   private:

    LAZY(NamedContext, Ref<Body>, body);
    LAZY(NamedContext, String, name);
    LAZY(NamedContext, String, qualified_name);
    LAZY(NamedContext, String, mangled_name);
    LAZY(NamedContext, Vector<String>, namespaces_names)
  };

  template <class T>
  Ref<T> Node::find_parent(Predicate<T> p_predicate) const {
    Ref<Node> current = get_parent();

    while (current) {
      Ref<T> casted = std::dynamic_pointer_cast<T>(current);
      if (casted != nullptr && p_predicate(casted)) {
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
  Ref<T> Context::find_ancestor(StemExplorationType p_type, Predicate<T> p_predicate) const {
    Ref<Node> current = get_parent();

    while (current) {
      Ref<T> current_t = current->as<T>();
      if (current_t && p_predicate(current_t)) {
        return current_t;
      }

      switch (p_type) {
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
  Ref<T> Context::get_child(SignedIndex p_idx) const {
    const Ref<Node> node = get_child(p_idx);
    if (!node) {
      return nullptr;
    }

    Ref<T> node_t = node->as<T>();
    if (!node_t) {
      return nullptr;
    }

    return node_t;
  }

  template <typename T>
  Ref<T> Context::find_chain(Predicate<T> p_predicate) const {
    return find_child<T>(0, p_predicate);
  }

  template <typename T, typename C, typename... Args>
  Ref<T> Context::find_chain(Predicate<T> p_predicate) const {
    Ref<C> current = find_child<C>();
    if (!current) {
      return nullptr;
    }

    return current->template find_chain<T, Args...>(p_predicate);
  }

  template <typename T>
  Ref<T> Context::find_child(Index p_start_idx, Predicate<T> p_predicate) const {
    if (p_start_idx >= _children.size()) {
      return nullptr;
    }

    auto itr = std::next(_children.begin(), p_start_idx);

    while (itr != _children.end()) {
      Ref<T> tChild = std::dynamic_pointer_cast<T>(*itr);
      if (tChild && p_predicate(tChild)) {
        return tChild;
      }
      ++itr;
    }

    return nullptr;
  }

  template <class T>
  Ref<T> Context::find_descendant(BranchExplorationType p_order, Predicate<T> p_predicate) const {
    switch (p_order) {
      case DFS:
        for (const Ref<Node>& child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t && p_predicate(child_t)) {
            return child_t;
          } else if (child->is<Context>()) {
            Ref<T> child_res = child->as<Context>()->find_descendant<T>(p_order, p_predicate);
            if (child_res && p_predicate(child_res)) {
              return child_res;
            }
          }
        }
        break;
      case BFS:
        for (const Ref<Node>& child : _children) {
          Ref<T> child_t = child->as<T>();
          if (child_t && p_predicate(child_t)) {
            return child_t;
          }
        }
        for (const Ref<Node>& child : _children) {
          if (child->is<Context>()) {
            if (Ref<T> child_res = child->as<Context>()->find_descendant<T>(p_order, p_predicate)) {
              return child_res;
            }
          }
        }
        break;
    }
    return nullptr;
  }

  template <class T>
  Ref<T> Context::find_previous_sibling(Predicate<T> p_predicate) const {
    if (get_parent() == nullptr) {
      return nullptr;
    }

    Ref<Node> current = get_previous_sibling();
    while (current) {
      Ref<T> current_t = current->as<T>();
      if (current_t && p_predicate(current_t)) {
        return current_t;
      }
      current = current->get_previous_sibling();
    }

    return nullptr;
  }

  template <class T, typename... Args>
  Ref<T> Context::create_child(Args&&... args) {
    Ref<T> child = ExecutionContext::instance()->get_node_db()->create<T>(std::forward<Args>(args)...);
    add_child(child);
    return child;
  }

  template <class T, typename... Args>
  Builder<T, Args...> Context::build_child(Args&&... args) {
    return Builder<T, Args...>(this->as<Context>(), std::forward<Args>(args)...);
  }

  template <class T>
  void find_recursive_helper(Node* node, bool recursive, Vector<Ref<T>>& results, Predicate<T> predicate) {
    Ref<T> node_t = node->as<T>();
    Ref<Context> node_context = node->as<Context>();

    if (node_t && predicate(node_t)) {
      results.push_back(node_t);
    }

    if (recursive && node_context) {
      for (const Ref<Node>& child : *node_context) {
        find_recursive_helper(child.get(), recursive, results, predicate);
      }
    }
  }

  template <class T>
  Vector<Ref<T>> Context::find_children(bool p_recursive, Predicate<T> p_predicate) const {
    Vector<Ref<T>> results;
    for (const Ref<Node>& child : _children) {
      find_recursive_helper(child.get(), p_recursive, results, p_predicate);
    }
    return results;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::Builder(Args... args) {
    _created = node_new<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::Builder(Ref<Context> parent, Args... args) {
    _created = node_new<T>(std::forward<Args>(args)...);
    if (parent) {
      parent->add_child(_created);
    }
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::operator Ref<T>() {
    return _created;
  }

  template <typename T, typename... Args>
  template <typename B, typename>
  Builder<T, Args...>::operator Ref<B>() {
    return _created->template as<B>();
  }

  template <typename T, typename... Args>
  Builder<T, Args...>::operator Ref<Node>() {
    return std::dynamic_pointer_cast<Node>(_created);
  }

  template <typename T, typename... Args>
  template <typename B, typename... BArgs>
  Builder<T, Args...>& Builder<T, Args...>::with_child(BArgs... child_args) {
    _created->add_child(node_new<B>(std::forward<BArgs>(child_args)...));
    return *this;
  }

  template <typename T, typename... Args>
  template <typename B, typename... BArgs>
  Builder<T, Args...>& Builder<T, Args...>::with_child_ref(Ref<B>* ptr, BArgs... child_args) {
    Ref<B> child = node_new<B>(std::forward<BArgs>(child_args)...);
    _created->add_child(child);
    if (ptr) {
      *ptr = child;
    }
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_child_ref(Ref<Node>* p_node, Ref<Node> p_child) {
    _created->add_child(p_child);
    if (p_node) {
      *p_node = p_child;
    }
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_child(Ref<Node> p_child) {
    _created->add_child(p_child);
    return *this;
  }

  template <typename T, typename... Args>
  Builder<T, Args...>& Builder<T, Args...>::with_children(std::initializer_list<Ref<Node>>&& p_children) {
    for (Ref<Node> child : p_children) {
      _created->add_child(child);
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

}

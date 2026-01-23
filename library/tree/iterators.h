
#pragma once
#include "syntax/context.h"

namespace GodotObjectCompiler {
  class Context;
  template <typename T>
  class BranchIterator {
   public:
    BranchIterator() = default;
    BranchIterator(Context* root, BranchExplorationType);

    bool operator==(const BranchIterator& other) const;
    bool operator!=(const BranchIterator& end) const;
    T*& operator*();
    T** operator->() const;
    BranchIterator& operator++();
    BranchIterator operator++(int);

    BranchIterator begin();
    BranchIterator end();

   private:
    bool forward_step();
    bool downward_step();
    bool upward_step();

    bool is_done = true;
    T* last_valid = nullptr;

    BranchExplorationType type;

    Context* root = nullptr;
    Context* current_parent = nullptr;
    Node* last_node = nullptr;
    Context::ChildIterator itr{};
  };

  template <typename T>
  BranchIterator<T>::BranchIterator(Context* _root, BranchExplorationType _type) {
    root = _root;
    type = _type;
    is_done = false;
  }

  template <typename T>
  bool BranchIterator<T>::operator==(const BranchIterator& other) const {
    if (is_done && other.is_done) {
      return true;
    }
    return (current_parent == other.current_parent) && (itr == other.itr);
  }
  template <typename T>
  bool BranchIterator<T>::operator!=(const BranchIterator& end) const {
    return !(*this == end);
  }

  template <typename T>
  T*& BranchIterator<T>::operator*() {
    return last_valid;
  }

  template <typename T>
  T** BranchIterator<T>::operator->() const {
    return &last_valid;
  }

  template <typename T>
  BranchIterator<T>& BranchIterator<T>::operator++() {
    if (is_done) {
      return *this;
    }

    if (itr != root->begin()) {
      forward_step();
    }

    while (downward_step() || forward_step() || upward_step()) {
      if (itr != current_parent->end()) {
        last_node = (*itr);
        // print_ln(last_node->to_string());
        T* node = (*itr)->as<T>();
        if (node) {
          last_valid = node;
          return *this;
        }
      }
    }

    *this = {};
    return *this;
  }

  template <typename T>
  BranchIterator<T> BranchIterator<T>::operator++(int steps) {
    while (steps > 0) {
      operator++();
      --steps;
    }
    return *this;
  }

  template <typename T>
  BranchIterator<T> BranchIterator<T>::begin() {
    current_parent = root;
    if (!current_parent || current_parent->get_child_count() == 0) {
      is_done = true;
      return *this;
    }

    is_done = false;
    itr = current_parent->begin();
    operator++();
    return *this;
  }

  template <typename T>
  BranchIterator<T> BranchIterator<T>::end() {
    return {};
  }

  template <typename T>
  bool BranchIterator<T>::forward_step() {
    if (itr == current_parent->end()) {
      return false;
    }

    ++itr;
    return true;
  }

  template <typename T>
  bool BranchIterator<T>::downward_step() {
    if (!last_node) {
      return false;
    }

    Context* ctx = last_node->as<Context>();
    if (!ctx) {
      return false;
    }

    last_node = nullptr;
    current_parent = ctx;
    itr = ctx->begin();

    return true;
  }

  template <typename T>
  bool BranchIterator<T>::upward_step() {
    if (current_parent == root) {
      return false;
    }

    do {
      Context* prev = current_parent;
      current_parent = current_parent->get_parent();
      itr = std::find(current_parent->begin(), current_parent->end(), prev);

      if (current_parent == root) {
        return false;
      }

      ++itr;
    } while (itr == current_parent->end());

    return true;
  }

}  // namespace GodotObjectCompiler

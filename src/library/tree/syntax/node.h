/**************************************************************************/
/* node.h                                                                 */
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

#include "library/core/core.h"
#include "library/core/reader_writer.h"
#include "library/execution_context.h"
#include "library/node_db.h"

namespace GodotObjectCompiler {

  class Include;
  class Context;
  class Node;

  class Node : public std::enable_shared_from_this<Node> {
   public:

    virtual String get_type() const { return "Node"; }

    virtual String to_string() const { return get_type(); }

    virtual bool copy_to(const Ref<Node>& p_other) const {
      UNUSED(p_other);
      return true;
    }

    virtual Ref<Node> create() { return ExecutionContext::instance()->get_node_db()->create<Node>(); }

    static String get_type_static() { return "Node"; }

    Node() = default;
    virtual ~Node() = default;

    Ref<Node> clone();

    virtual void write_to(IStructuredWriter* p_writer);
    virtual void read_from(IStructuredReader* p_reader);

    bool has_parent() const;
    bool has_next_sibling() const;
    bool has_previous_sibling() const;

    String pretty_print() const;
    String print_pretty_and_get_child_line(const Ref<Node>& p_child, Size& p_line) const;

    UID get_id() const;
    Index get_index() const;
    Index get_depth() const;
    Ref<Context> get_parent() const;
    Ref<Node> get_root();

    void reparent(const Ref<Context>& p_new_parent);

    Ref<Node> get_sibling(int p_offset) const;
    Ref<Node> get_next_sibling() const;
    Ref<Node> get_previous_sibling() const;

    template <typename T>
    static bool default_node_predicate(Ref<T>) {
      return true;
    }

    template <class T>
    Ref<T> find_parent(Predicate<T> p_predicate = default_node_predicate<T>) const;

    template <class T>
    Ref<T> find_previous_sibling();

    template <class T>
    bool is() const;

    template <class T>
    Ref<T> as();

    template <class T>
    Ref<const T> const_as() const;

   private:

    mutable Ref<Node> _root = nullptr;
    WeakRef<Context> _parent;
    UID _id = INVALID_ID;
    Index _index = INVALID_INDEX;

    friend class NodeDB;
    friend class Context;
  };

  template <typename T>
  Ref<Node> default_construct();

  template <typename T, typename... Args>
  Ref<T> node_new(Args&&... args) {
    return ExecutionContext::instance()->get_node_db()->create<T>(std::forward<Args>(args)...);
  }

}

template <class T>
bool GodotObjectCompiler::Node::is() const {
  return std::dynamic_pointer_cast<const T>(shared_from_this()) != nullptr;
}

template <class T>
GodotObjectCompiler::Ref<T> GodotObjectCompiler::Node::as() {
  return std::dynamic_pointer_cast<T>(shared_from_this());
}

template <class T>
GodotObjectCompiler::Ref<const T> GodotObjectCompiler::Node::const_as() const {
  return std::dynamic_pointer_cast<const T>(shared_from_this());
}

template <typename T>
GodotObjectCompiler::Ref<GodotObjectCompiler::Node> GodotObjectCompiler::default_construct() {
  return ExecutionContext::instance()->get_node_db()->create<T>();
}

template <typename T>
GodotObjectCompiler::Ref<T> GodotObjectCompiler::INodeReader::read_from_file(const String& p_path) {
  Ref<Node> result = read_from_file(p_path);
  if (!result) {
    return nullptr;
  }

  return result->template as<T>();
}

#define NODE_TYPE(type)                                                                                    \
 public:                                                                                                   \
                                                                                                           \
  type() = default;                                                                                        \
  virtual String get_type() const override { return #type; }                                               \
  static String get_type_static() { return #type; }                                                        \
  static Ref<Node> create_static() { return ExecutionContext::instance()->get_node_db()->create<type>(); } \
  virtual Ref<Node> create() override { return type::create_static(); }                                    \
  static inline bool __registered__##type##__ =                                                            \
      ExecutionContext::instance()->get_node_db()->register_node_constructor(                              \
          #type, &GodotObjectCompiler::default_construct<type>);

#define COPY_GUARD(type, parent)     \
  auto target = p_other->as<type>(); \
  if (!target) {                     \
    return false;                    \
  }                                  \
  if (!parent::copy_to(target)) {    \
    return false;                    \
  }

#define COPY_LAZY(name) target->_##name##_lazy = name();

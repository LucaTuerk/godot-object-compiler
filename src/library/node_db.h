/**************************************************************************/
/* node_db.h                                                              */
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
#include "core/core.h"

namespace GodotObjectCompiler
{

class Node;
class ExecutionContext;

class NodeDB
{
private:
  struct Private {
  };

public:
  UID request_id_change(UID p_from, UID p_to = INVALID_ID);

  template <typename T, typename... Args> Ref<T> create(Args&&... p_args);

  template <typename T, typename... Args>
  Ref<T> create_with_id(UID p_uid, Args&&... p_args);

  template <typename T> Ref<T> get(UID p_uid);

  using NodeCreator = Creator<Node>;

  bool register_node_constructor(const String& p_name, NodeCreator p_creator);

  static Ref<Node> create(const String& p_type);

  NodeDB() = delete;
  NodeDB(Private) {}
  ~NodeDB();

private:
  static inline HashSet<UID> _uids{};

  static UID _generate_unique_id();

  static bool _has_uid(UID p_uid);

  Dictionary<UID, WeakRef<Node>> _nodes;

  Dictionary<String, NodeCreator> _node_constructors;

  friend ExecutionContext;
};

template <typename T, typename... Args> Ref<T> NodeDB::create(Args&&... p_args)
{
  return create_with_id<T, Args...>(
      _generate_unique_id(), std::forward<Args>(p_args)...);
}

template <typename T, typename... Args>
Ref<T> NodeDB::create_with_id(UID p_uid, Args&&... p_args)
{
  p_uid = p_uid == INVALID_ID ? _generate_unique_id() : p_uid;

  if (const auto itr = _nodes.find(p_uid); itr != _nodes.end()) {
    if (const Ref<Node> other = itr->second.lock(); other != nullptr) {
      print_err("Node with uid already exits");
      return nullptr;
    }
  }

  Ref<T> node = make_ref<T>(std::forward<Args>(p_args)...);
  node->_id = p_uid;
  _nodes[p_uid] = node;
  return node;
}

template <typename T> Ref<T> NodeDB::get(UID p_uid)
{
  if (p_uid == INVALID_ID) {
    return nullptr;
  }

  if (const auto itr = _nodes.find(p_uid); itr != _nodes.end()) {
    return std::dynamic_pointer_cast<T>(itr->second.lock());
  }

  return nullptr;
}

} // namespace GodotObjectCompiler

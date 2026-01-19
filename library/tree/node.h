#pragma once

#include "../core/io/reader_writer.h"
#include "library/core/core.h"
#include "library/core/db.h"
#include "library/core/lazy.h"

#define NODE_TYPE(type)                                                               \
public:                                                                               \
	virtual String get_type() const override {                                        \
		return #type;                                                                 \
	}                                                                                 \
	static String get_type_static() {                                                 \
		return #type;                                                                 \
	}                                                                                 \
	virtual Node *create() override {                                                 \
		return NodeDB::get_instance()->create<type>();                                \
	}                                                                                 \
	static inline bool __registered__##type##__ =                                     \
			DB::register_node_constructor(#type, &GodotObjectCompiler::create<type>); \
                                                                                      \
private:

#define COPY_GUARD(type, parent) \
	type* target = other->as<type>(); \
	if(!target) {return false;} \
	if(!parent::copy_to(target)) {return false;}

#define COPY_LAZY(name) target->_##name##_lazy = name();

namespace GodotObjectCompiler {

enum TreeIterationOrder {
	BFS,
	DFS,
};

class Context;
class Node;

class NodeDB {
public:
	static NodeDB *get_instance() {
		static NodeDB instance;
		return &instance;
	}

	template <typename T>
	T *create(UID uid = INVALID_ID);

	template <typename T>
	T *get(UID uid);

	template <typename T>
	Vector<Node *> &get_of_type();

private:
	NodeDB() = default;
	~NodeDB();

	static UID _generate_unique_id();

	Dictionary<UID, Node *> _nodes;
	Dictionary<TypeIndex, Vector<Node *>> _type_nodes;
};

class Node {
public:
	virtual String get_type() const { return "Node"; }
	virtual String to_string() const { return get_type(); }
	virtual bool copy_to(Node *other) const { return true; }
	virtual Node *create() { return NodeDB::get_instance()->create<Node>(); }
	static String get_type_static() { return "Node"; }

	Node() = default;
	virtual ~Node() = default;

	Node *clone();

	virtual void write_to(IWriter *writer);
	virtual void read_from(IReader *reader);

	String pretty_print() const;

	UID get_id() const;
	Index get_index() const;
	Index get_depth() const;
	Context *get_parent() const;

	template <class T>
	T *find_parent();

	template <class T>
	T *find_previous_sibling();

	template <class T>
	bool is();

	template <class T>
	T *as();

	void reparent(Context *new_parent);

	Node *get_sibling(int p_offset) const;
	Node *get_next_sibling() const;
	Node *get_previous_sibling() const;

private:
	Context *_clone_parent = nullptr;
	Context *_parent = nullptr;
	UID _id = INVALID_ID;
	Index _index = INVALID_INDEX;

	friend class NodeDB;
	friend class Context;
};

template <typename T>
Node *create();

class Context : public Node {
	NODE_TYPE(Context)

public:
	Context() = default;
	~Context();
	bool copy_to(Node *other) const override;

	void add_child(Node *p_child);
	void remove_child(Node *p_child);
	void replace_child(Node *p_child, Node *p_new_child);

	Size get_child_count() const;
	Node *get_child(SignedIndex p_idx) const;
	Node *get_child_strict(SignedIndex p_idx) const;
	Vector<Node *> &get_children();

	template <class T>
	T *find_ancestor();

	template <class T>
	T *find_decendant(TreeIterationOrder order = BFS);

	template <class T>
	T *create_child();

	template <typename T>
	T *find_child(Index p_start_idx = 0) const;

	virtual void write_to(IWriter *writer) override;

private:
	Vector<Node *> _children;
};

class NamedContext : public Context {
	NODE_TYPE(NamedContext)
	LAZY(NamedContext, String, name);
	LAZY(NamedContext, String, qualified_name);

public:
	bool copy_to(Node *other) const override;
};
} //namespace GodotObjectCompiler

template <typename T>
T *GodotObjectCompiler::Context::find_child(Index p_start_idx) const {
	for (Index i = p_start_idx; i < _children.size(); ++i) {
		T *child = dynamic_cast<T *>(_children[i]);
		if (child) {
			return child;
		}
	}

	return nullptr;
}

template <class T>
T *GodotObjectCompiler::Node::find_parent() {
	Node *current = _parent;

	while (current) {
		if (T *casted = dynamic_cast<T *>(current)) {
			return casted;
		}
		current = current->get_parent();
	}

	return nullptr;
}
template <class T>
T *GodotObjectCompiler::Node::find_previous_sibling() {
	Node *current = get_previous_sibling();

	while (current) {
		if (T *casted = dynamic_cast<T *>(current)) {
			return casted;
		}
		current = current->get_previous_sibling();
	}

	return nullptr;
}

template <class T>
bool GodotObjectCompiler::Node::is() {
	return dynamic_cast<T *>(this) != nullptr;
}

template <class T>
T *GodotObjectCompiler::Node::as() {
	return dynamic_cast<T *>(this);
}

template <typename T>
GodotObjectCompiler::Node *GodotObjectCompiler::create() {
	return dynamic_cast<Node *>(new T());
}

template <class T>
T *GodotObjectCompiler::Context::find_ancestor() {
	Context *current = _parent;

	while (current) {
		if (current->is<T>()) {
			return current->as<T>();
		}
		current = current->get_parent();
	}

	return nullptr;
}

template <class T>
T *GodotObjectCompiler::Context::find_decendant(TreeIterationOrder order) {
	switch (order) {
		case DFS:
			for (auto child : _children) {
				if (child->is<T>()) {
					return child->as<T>();
				} else if (child->is<Context>()) {
					T *child_res = child->as<Context>()->find_decendant<T>(order);
					if (child_res) {
						return child_res;
					}
				}
			}
			break;
		case BFS:
			for (auto child : _children) {
				if (child->is<T>()) {
					return child->as<T>();
				}
			}
			for (auto child : _children) {
				if (child->is<Context>()) {
					T *child_res = child->as<Context>()->find_decendant<T>(order);
					if (child_res) {
						return child_res;
					}
				}
			}
			break;
	}
	return nullptr;
}

template <typename T>
T *GodotObjectCompiler::Context::create_child() {
	T *child = NodeDB::get_instance()->create<T>();
	add_child(child);
	return child;
}

template <typename T>
T *GodotObjectCompiler::NodeDB::create(UID uid) {
	if (_nodes.find(uid) != _nodes.end()) {
		return dynamic_cast<T *>(_nodes[uid]);
	}

	T *node = new T();
	uid = uid == INVALID_ID ? _generate_unique_id() : uid;
	node->_id = uid;
	_nodes[uid] = node;
	_type_nodes[typeid(T)].push_back(node);
	return node;
}

template <typename T>
T *GodotObjectCompiler::NodeDB::get(UID uid) {
	if (_nodes.find(uid) != _nodes.end()) {
		return dynamic_cast<T *>(_nodes[uid]);
	}

	return nullptr;
}

template <typename T>
Vector<GodotObjectCompiler::Node *> &GodotObjectCompiler::NodeDB::get_of_type() {
	return _type_nodes[typeid(T)];
}

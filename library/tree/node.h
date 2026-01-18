#pragma once

#include "../core/io/reader_writer.h"
#include "library/core/core.h"
#include "library/core/db.h"

#define NODE_TYPE(type) \
    public: \
        virtual String get_type() const override { return #type; } \
        static String get_type_static() { return #type; }\
		static inline bool __registered__##type##__ = \
			DB::register_node_constructor(#type, &GodotObjectCompiler::create<type>); \
    private:\

namespace GodotObjectCompiler {

    class Context;

    class Node {
        public:
            virtual String get_type() const { return "Node"; }
            static String get_type_static() { return "Node"; }

            Node() = default;
            virtual ~Node() = default;

            virtual void write_to(IWriter *writer);
    		virtual void read_from(IReader *reader);

            UID get_id() const;
    		Index get_index() const;
            Context* get_parent() const;

    		Node* get_sibling(int p_offset) const;
    		Node* get_next_sibling() const;
    		Node* get_previous_sibling() const;

        private:
            Context* _parent = nullptr;
            UID _id = INVALID_ID;
    		Index _index = INVALID_INDEX;

            friend class NodeDB;
            friend class Context;
    };


	template<typename T>
	Node* create();

    class Context : public Node {
        NODE_TYPE(Context)

        public:
            Context() = default;
            ~Context();

            void add_child(Node* p_child);
            Size get_child_count() const;
            Node* get_child(Index p_idx) const;

            template <class T>
    		T* create_child();

    		template<typename T>
			T* find_child(Index p_start_idx = 0) const;

            virtual void write_to(IWriter *writer) override;

        private:
            Vector<Node*> _children;
    };

    class NodeDB {
        public:
        static NodeDB* get_instance() {
            static NodeDB instance;
            return &instance;
        }

        template<typename T>
        T* create(UID uid = INVALID_ID);

        template<typename T>
        T* get(UID uid);

        template<typename T>
        Vector<Node*>& get_of_type();

        private:
        NodeDB() = default;
        ~NodeDB();

		static UID _generate_unique_id();

        Dictionary<UID, Node*> _nodes;
        Dictionary<TypeIndex, Vector<Node*>> _type_nodes;
    };
} //namespace GodotObjectCompiler

	template <typename T>
	T *GodotObjectCompiler::Context::find_child(Index p_start_idx) const {
		for (Index i = p_start_idx; i < _children.size(); ++i) {
			T* child = dynamic_cast<T>(_children[i]);
			if (child) {
				return child;
			}
		}

		return nullptr;
	}

	template <typename T>
	GodotObjectCompiler::Node *GodotObjectCompiler::create() {
		return dynamic_cast<Node *>(new T());
	}

	template <typename T>
	T* GodotObjectCompiler::Context::create_child() {
	    T* child = NodeDB::get_instance()->create<T>();
	    add_child(child);
	    return child;
	}

	template<typename T>
	T* GodotObjectCompiler::NodeDB::create(UID uid) {
	    if (_nodes.find(uid) != _nodes.end()) {
	        return dynamic_cast<T*>(_nodes[uid]);
	    }

	    T* node = new T();
	    uid = uid == INVALID_ID ? _generate_unique_id() : uid;
	    node->_id = uid;
	    _nodes[uid] = node;
	    _type_nodes[typeid(T)].push_back(node);
	    return node;
	}

	template<typename T>
	T* GodotObjectCompiler::NodeDB::get(UID uid) {
	    if (_nodes.find(uid) != _nodes.end()) {
	        return dynamic_cast<T*>(_nodes[uid]);
	    }

	    return nullptr;
	}

	template<typename T>
	Vector<GodotObjectCompiler::Node*>& GodotObjectCompiler::NodeDB::get_of_type() {
	    return _type_nodes[typeid(T)];
	}

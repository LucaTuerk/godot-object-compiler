#pragma once

#include "library/core/core.h"
#include "library/core/config.h"

#define NODE_TYPE(type) \
    public: \
        String get_type() const override { return #type; } \
        static String get_type_static() { return #type; }\
    private:\

namespace GodotObjectCompiler {

    class Context;

    class Node {
        public:
            virtual String get_type() const { return "Node"; }
            static String get_type_static() { return "Node"; }

            Node() = default;
            virtual ~Node() = default;
            virtual void dump_properties(Config& config);

            UID get_id() const;
            Context* get_parent() const;

        private:
            Context* _parent = nullptr;
            UID _id = INVALID_ID;

            friend class NodeDB;
            friend class Context;
    };

    class Context : public Node {
        NODE_TYPE(Context)
        public:
            Context() = default;
            ~Context();

            void add_child(Node* p_child);
            Size get_child_count() const;
            Node* get_child(Size p_idx) const;

            template <class T>
            T* create_child();

            virtual void dump_properties(Config& config) override;

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

        UID _generate_unique_id();

        Dictionary<UID, Node*> _nodes;
        Dictionary<TypeIndex, Vector<Node*>> _type_nodes;
    };
}

template<typename T>
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

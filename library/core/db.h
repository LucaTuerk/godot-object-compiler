#pragma once
#include "core.h"

namespace GodotObjectCompiler {
	class Node;
	class Namespace;

    class DB {
        public:
			using NodeCreatorFunc = Node*(*)();

    		static DB init(Namespace* root = nullptr);
            static DB read_from_config(const String& path);

    		static bool register_node_constructor(const String& name,NodeCreatorFunc);
			static Node * create(const String& type);

            void write_to_config(const String& path) const;
            Namespace* get_root() const;

        private:
            Namespace* _root = nullptr;

    		static inline Dictionary<String, NodeCreatorFunc> _node_constructors;
    };
}

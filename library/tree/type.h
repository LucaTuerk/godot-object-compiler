#pragma once

#include "node.h"

namespace GodotObjectCompiler {
    class Reference : public Node {
        NODE_TYPE(Reference)
    };

    class Pointer : public Node {
        NODE_TYPE(Pointer)
    };

    class Type : public NamedContext {
        NODE_TYPE(Type)
    };

	class PlaceholderType : public Node {
		NODE_TYPE(PlaceholderType)
	};
} //namespace GodotObjectCompiler

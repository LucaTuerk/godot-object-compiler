#pragma once
#include "library/core/config.h"
#include "node.h"

namespace GodotObjectCompiler {

	class AccessSpecifier : public Node {
		NODE_TYPE(AccessSpecifier)

	public:
		enum Type {
			PUBLIC,
			PRIVATE,
			PROTECTED
		};

		Type type = PUBLIC;

		void dump_properties(Config &config) override;
	};
	} //namespace GodotObjectCompiler

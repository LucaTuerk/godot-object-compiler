#pragma once
#include "../core/io/config.h"
#include "node.h"

namespace GodotObjectCompiler {

	class AccessSpecifier : public Node {
		NODE_TYPE(AccessSpecifier)

	public:
		bool copy_to(Node *other) const override;

		enum Type {
			PUBLIC,
			PRIVATE,
			PROTECTED
		};

		Type type = PUBLIC;

		String to_string() const override;

		void write_to(IWriter *writer) override;
	};
	} //namespace GodotObjectCompiler

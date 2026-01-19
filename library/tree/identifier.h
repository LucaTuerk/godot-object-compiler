#pragma once

#include "node.h"

namespace GodotObjectCompiler {
class Identifier : public Node {
	NODE_TYPE(Identifier)
	public:
		bool copy_to(Node *other) const override;

		String to_string() const override;
		virtual void write_to(IWriter *writer) override;

	public:
		String name;
	};
} //namespace GodotObjectCompiler

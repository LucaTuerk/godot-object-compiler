#include "access_specifier.h"

namespace GodotObjectCompiler {

	String AccessSpecifier::to_string() const {
		String value = "";

		switch (type) {
			case PUBLIC:
				value += "public";
				break;
			case PRIVATE:
				value += "private";
				break;
			case PROTECTED:
				value += "protected";
				break;
		}

		return "AccessSpecifier: " + value;
	}

	bool AccessSpecifier::copy_to(Node *other) const {
		COPY_GUARD(AccessSpecifier, Node);
		target->type = type;
		return true;
	}

	void AccessSpecifier::write_to(IWriter *writer) {
		Node::write_to(writer);
		writer->write("type", type);
	}

}
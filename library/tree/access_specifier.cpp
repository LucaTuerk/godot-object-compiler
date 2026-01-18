#include "access_specifier.h"

namespace GodotObjectCompiler {
	void AccessSpecifier::write_to(IWriter *writer) {
		Node::write_to(writer);
		writer->write("type", type);
	}
}
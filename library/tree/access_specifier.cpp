#include "access_specifier.h"

namespace GodotObjectCompiler {
	void AccessSpecifier::dump_properties(Config &config) {
		Node::dump_properties(config);
		config.set("type", type);
	}
}
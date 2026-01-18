#include "library/tree/identifier.h"

namespace GodotObjectCompiler {
    void Identifier::dump_properties(Config& config) {
        Node::dump_properties(config);
        config.set("name", name);
    }
}

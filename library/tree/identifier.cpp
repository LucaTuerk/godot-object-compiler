#include "library/tree/identifier.h"

namespace GodotObjectCompiler {
    void Identifier::write_to(IWriter *writer) {
        Node::write_to(writer);
        writer->write("name", name);
    }
}


#include "string_writer.h"

namespace GodotObjectCompiler {

  void StringWriter::write(const String& value) {
    stream << value;
  }

  String StringWriter::get_string() {
    return stream.str();
  }

}  // namespace GodotObjectCompiler

#include "string_writer.h"

#include <iostream>

namespace GodotObjectCompiler {

  void StreamWriter::write(const String& value) {
    if (value == "\n") {
      std::cout << value.length() << std::endl;
    }

    _stream << value;
    _current_length += value.length();
  }

  String StreamWriter::get_string() { return _stream.str(); }

  Size StreamWriter::current_length() { return _current_length; }
}  // namespace GodotObjectCompiler
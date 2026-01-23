#pragma once
#include <sstream>

#include "library/core/core.h"
#include "reader_writer.h"

namespace GodotObjectCompiler {

  class StreamWriter : public IStringWriter {
   public:
    void write(const String& value) override;
    String get_string() override;
    Size current_length() override;

   private:
    std::stringstream _stream;
    Size _current_length = 0;
  };

}  // namespace GodotObjectCompiler

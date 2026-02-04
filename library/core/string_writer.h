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

  class FileWriter : public IStringWriter {
   public:

    explicit FileWriter(const String& path, bool do_not_write_same_content = false);
    ~FileWriter() override;

    void write(const String& value) override;
    String get_string() override;
    Size current_length() override;

   private:

    String path;
    bool do_not_write_same_content = false;
    StreamWriter _stream;
    std::fstream _file;
  };

}  // namespace GodotObjectCompiler

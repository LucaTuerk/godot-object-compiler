//
// Created by luca on 19.01.26.
//

#include "output_file.h"

#include "library/core/string_writer.h"
#include "output.h"
#include "output_transformator.h"

namespace GodotObjectCompiler {

  void OutputFile::print() {
    StreamWriter writer;
    write_output(&writer);
  }

  void OutputFile::write_output(IStringWriter* writer) {
    if (!transformed) {
      transformed = OutputTransformator().transform(shared_from_this());
      if (!transformed) {
        return;
      }
    }
    transformed->get_output(writer);
  }

}  // namespace GodotObjectCompiler
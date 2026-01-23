//
// Created by luca on 19.01.26.
//

#include "output_file.h"

#include "library/core/string_writer.h"
#include "output.h"

namespace GodotObjectCompiler {
  void OutputFile::print() {
    StreamWriter writer;
    write_output(&writer);
    print_ln(writer.get_string());
  }

  void OutputFile::write_output(IStringWriter* writer) {
    for (Node* child : get_children()) {
      Writer::IOutputNode* output_node = child->as<Writer::IOutputNode>();
      if (output_node) {
        output_node->get_output(writer);
      }
    }
  }
}  // namespace GodotObjectCompiler
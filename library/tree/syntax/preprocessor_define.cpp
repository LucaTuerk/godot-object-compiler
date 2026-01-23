
#include "preprocessor_define.h"

namespace GodotObjectCompiler {

  String PreprocessorDefine::to_string() const { return "PreprocessorDefine: " + name; }

  bool PreprocessorDefine::copy_to(Node* other) const {
    COPY_GUARD(PreprocessorDefine, Node);
    target->name = name;
    return true;
  }

  void PreprocessorDefine::write_to(IStructuredWriter* writer) {
    Node::write_to(writer);
    writer->write("preprocessor_name", name);
  }

  void PreprocessorDefine::read_from(IStructuredReader* reader) {
    Node::read_from(reader);
    reader->read<String, String>("preprocessor_name");
  }

}  // namespace GodotObjectCompiler
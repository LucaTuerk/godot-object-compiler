
#include "preprocessor_define.h"

namespace GodotObjectCompiler {

  String PreprocessorDefine::to_string() const { return "PreprocessorDefine: " + name; }

  bool PreprocessorDefine::copy_to(Ref<Node> p_other) const {
    COPY_GUARD(PreprocessorDefine, Node);
    target->name = name;
    return true;
  }

  void PreprocessorDefine::write_to(IStructuredWriter* p_writer) {
    Node::write_to(p_writer);
    p_writer->write("preprocessor_name", name);
  }

  void PreprocessorDefine::read_from(IStructuredReader* p_reader) {
    Node::read_from(p_reader);
    p_reader->read<String, String>("preprocessor_name");
  }

}  // namespace GodotObjectCompiler
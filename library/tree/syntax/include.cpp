#include "include.h"

#include "library/core/db.h"
#include "library/core/helpers.h"
#include "library/execution_context.h"
#include "library/parser/parser.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  String Include::to_string() const {
    return is_system_include ? "Include: " + include_path : "Include: \"" + include_path + "\"";
  }

  void Include::read_from(IStructuredReader* reader) {
    // intentionally read from Node as we do to want to copy the children
    Node::read_from(reader);  // NOLINT(*-parent-virtual-call)
    include_path = reader->read<String, String>("include_path");
    is_system_include = reader->read<String, bool>("is_system_include");
  }

  void Include::write_to(IStructuredWriter* writer) {
    // intentionally write to Node as we do to want to copy the children
    Node::write_to(writer);  // NOLINT(*-parent-virtual-call)
    writer->write("include_path", include_path);
    writer->write("is_system_include", is_system_include);
  }

  bool Include::copy_to(Ref<Node> other) const {
    // intentionally copying from node as we do to want to copy the children
    COPY_GUARD(Include, Node)  // NOLINT(*-parent-virtual-call)
    target->include_path = include_path;
    target->is_system_include = is_system_include;
    return true;
  }

  Include::Error Include::_evaluate_lazy_get() {
    Ref<Node> result = ExecutionContext::instance()->get_include("", include_path);
    if (result) {
      add_child(result);
      return OK;
    } else {
      return ALREADY_INCLUDED;
    }
  }

}  // namespace GodotObjectCompiler
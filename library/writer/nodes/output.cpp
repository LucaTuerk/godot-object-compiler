#include "output.h"

namespace GodotObjectCompiler {
  namespace Writer {

    String Indent::get_output() {
      String child_result = "";

      for (Node* child : get_children()) {
        IOutputNode* output = child->as<IOutputNode>();
        if (output) {
          child_result += output->get_output();
        }
      }

      String result = "";
      Size total = total_amount();
      Size start = 0;
      do {
        Size prev_start = start;
        start = child_result.find("\n", start) + 1;
        for (Size i = 0; i < total; ++i) {
          result += " ";
        }
        result += child_result.substr(
            prev_start, start == String::npos ? start : start - prev_start);
      } while (start < String::npos);

      return result;
    }

    Size Indent::_total_amount_lazy_get() {
      Indent* parent = find_ancestor<Indent>();

      if (parent) {
        return parent->total_amount() + amount;
      }

      return amount;
    }

    Enclosing* Brackets() {
      return NodeDB::get_instance()->create<Enclosing>("(", ")");
    }

    Enclosing* SquareBrackets() {
      return NodeDB::get_instance()->create<Enclosing>("[", "]");
    }

    Enclosing* Braces() {
      return NodeDB::get_instance()->create<Enclosing>("{", "}");
    }

    Enclosing* Chevrons() {
      return NodeDB::get_instance()->create<Enclosing>("<", ">");
    }

    List* Lines() {
      return NodeDB::get_instance()->create<List>("\n", true, true);
    }

    List* Parameters() {
      return NodeDB::get_instance()->create<List>(", ", false, false);
    }

  }  // namespace Writer
}  // namespace GodotObjectCompiler
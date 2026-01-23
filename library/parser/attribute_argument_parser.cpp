
#include "attribute_argument_parser.h"

#include "../tree/syntax/all.h"
#include "../tree/syntax/function.h"
#include "../tree/syntax/identifier.h"
#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  void AttributeArgumentParser::setup_for_macro(const String& macro_name) {
    parameters = AttributeDB::instance()->get_parameter_types(macro_name);
  }

  Node* AttributeArgumentParser::parse(const String& input) {
    Parameters* result = ExecutionContext::instance()->get_node_db()->create<Parameters>();

    for (const String& argument : string_split(input, ",")) {
      if (!argument.empty()) {
        result->create_child<Parameter>()->create_child<Identifier>(argument);
      }
    }

    return result;
  }
}  // namespace GodotObjectCompiler
#pragma once
#include "library/attribute_db.h"
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"
#include "parser.h"

namespace GodotObjectCompiler {

  class IAttributeArgumentParser {
   public:

    virtual Ref<ParserError> parse_attribute_arguments(const String& content, Ref<Context> target) = 0;

    static Vector<String> split_arguments(const String& content);
    static String get_inner_arguments(const String& content);
  };

}  // namespace GodotObjectCompiler

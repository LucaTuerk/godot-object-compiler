#pragma once
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler {

  class IAttributeArgumentParser {
   public:

    virtual ~IAttributeArgumentParser() = default;

    virtual Ref<ParserError> parse_attribute_arguments(const String& p_content, Ref<Context> p_target) = 0;

    static Vector<String> split_arguments(const String& p_content);

    static void split_outer_inner(const String& p_content, String& r_outer, String& r_inner);

    static Vector<String> split_flags(const String& p_content);
  };

}  // namespace GodotObjectCompiler

#define ARGUMENT_PARSER(type) \
  static String() get_type_static() { return #type; }

#define ATTRIBUTE_PARSE_ERROR(message) return node_new<ParserError>(ERROR, message);
